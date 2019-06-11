#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>

#include <unistd.h>
#include <sys/types.h>   // socket
#include <sys/socket.h>  // socket
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "Utils/selector.h"
#include "proxy5nio.h"
#include "Utils/log.h"

static bool done = false;

static void
sigterm_handler(const int signal) {
    printf("signal %d, cleaning up and exiting\n", signal);
    done = true;
}

int
main(const int argc, const char **argv) {
    if (argc != 2) {
        printf("Parameter: <Server Port>");
        return 1;
    }

    unsigned port = atoi(argv[1]);

    // no tenemos nada que leer de stdin
    close(0);

    logger_init();
    log_debug("hola");

    const char *err_msg = NULL;
    selector_status ss = SELECTOR_SUCCESS;
    fd_selector selector = NULL;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    const int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server < 0) {
        err_msg = "unable to create socket";
        goto finally;
    }

    fprintf(stdout, "Listening on TCP port %d\n", port);

    // man 7 ip. no importa reportar nada si falla.
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int));

    if (bind(server, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        err_msg = "unable to bind socket";
        goto finally;
    }

    if (listen(server, 1) < 0) {
        err_msg = "unable to listen";
        goto finally;
    }

    // registrar sigterm es útil para terminar el programa normalmente.
    // esto ayuda mucho en herramientas como valgrind.
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigterm_handler);
    signal(SIGPIPE, SIG_IGN); // ignoro los SIGPIPE de esta manera ya que mac no soporta MSG_NOSIGNAL

    if (selector_fd_set_nio(server) == -1) {
        err_msg = "getting server socket flags";
        goto finally;
    }
    const struct selector_init conf = {
            .signal = SIGALRM,
            .select_timeout = {
                    .tv_sec  = 10,
                    .tv_nsec = 0,
            },
    };
    if (0 != selector_init(&conf)) {
        err_msg = "initializing selector";
        goto finally;
    }

    selector = selector_new(1024);
    if (selector == NULL) {
        err_msg = "unable to create selector";
        goto finally;
    }
    const struct fd_handler proxyv5 = {
            .handle_read       = proxyv5_passive_accept,
            .handle_write      = NULL,
            .handle_close      = NULL, // nada que liberar
    };
    ss = selector_register(selector, server, &proxyv5,
                           OP_READ, NULL);
    if (ss != SELECTOR_SUCCESS) {
        err_msg = "registering fd";
        goto finally;
    }
    for (; !done;) {
        err_msg = NULL;
        ss = selector_select(selector);
        if (ss != SELECTOR_SUCCESS) {
            err_msg = "serving";
            goto finally;
        }
    }
    if (err_msg == NULL) {
        err_msg = "closing";
    }

    int ret = 0;
    finally:
    if (ss != SELECTOR_SUCCESS) {
        fprintf(stderr, "%s: %s\n", (err_msg == NULL) ? "" : err_msg,
                ss == SELECTOR_IO
                ? strerror(errno)
                : selector_error(ss));
        ret = 2;
    } else if (err_msg) {
        perror(err_msg);
        ret = 1;
    }
    if (selector != NULL) {
        selector_destroy(selector);
    }
    selector_close();

    proxyv5_pool_destroy();

    if (server >= 0) {
        close(server);
    }
    return ret;
}
