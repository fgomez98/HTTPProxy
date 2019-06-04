#include <stdlib.h>
#include "hpcpRequest.h"
#define N_COMMANDS 0x03

extern enum hpcp_request_state
request_parser_feed (struct hpcp_request_parser* p, const uint8_t c) {
    enum hpcp_request_state next;

    switch(p->state) {
        case hpcp_request_cmd:
            next = cmd_parser(c, p);
            break;
        case hpcp_request_nargs:
           // next = nargs_parser(c, p);
            break;
        case hpcp_request_arglen:
           // next = arglen_parser(c, p);
            break;
        case hpcp_request_arg:
           // next = arg_parser(c, p);
            break;
        case hpcp_request_done:
        case hpcp_request_error:
        case hpcp_request_error_unsupported_version:
        case hpcp_request_error_invalid_cmd:
        case hpcp_request_error_invalid_args:
        case hpcp_request_error_invalid_credentials:
        case hpcp_request_error_invalid_transformation_program:
            next = p->state;
            break;
        default:
            next = hpcp_request_error;
            break;
    }
    return p->state = next;
}

static enum hpcp_request_state
cmd_parser(const uint8_t c, struct hpcp_request_parser* p) {
    if(c > N_COMMANDS) {
        return hpcp_request_error_invalid_cmd;
    }
    p->request->cmd = c;
    //Retorna el state al que pasa el parser dado que no hubo error en este estado.
    return hpcp_request_nargs;
}