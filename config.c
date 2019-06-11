#include <stdlib.h>
#include "config.h"

#define VER_MAJOR 1
#define VER_MINOR 0

bool is_valid_version(const uint8_t *version, size_t bytes);

hpcp_request *config(hpcp_request *req) { // todo confimrar si puntero o no
    // preparamos la respuesta
    hpcp_request *res = malloc(sizeof(hpcp_request));
    res->cmd = ERR;
    // filtramos por el cmd del protocolo
    switch(req->cmd) {
        case HELLO:
            handle_hello(req, res);
        case AUTH:
            handle_auth(req, res);
        case CLOSE:
            handle_close(req, res);
        case GET:
            handle_get(req, res);
        case SET:
            handle_set(req, res);
        case ERR:break;
    }
    return res;
};


bool is_valid_version(const uint8_t *version, size_t bytes) {
    return bytes != 2 && version[0] != VER_MAJOR && version[1] != VER_MINOR;
}

void handle_hello(hpcp_request *req, hpcp_request *res) {
    // chequiamos la version
    if(is_valid_version(req->args[1], req->arg_sizes[1])) {
        res->cmd = ERR;
        return ;
    }
    res->cmd = req->cmd;
    res->args = malloc(2*sizeof(uint8_t));
}
