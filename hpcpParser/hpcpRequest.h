#include <stdint.h>
#ifndef HPCP_REQUEST_H
#define HPCP_REQUEST_H
/*   The HPCP request is formed as follows:
 *
 *       +-----+-------+----------+----------+-----+-----------+--------+
 *       | CMD |N-ARGS | ARGLEN 1 | ARG1     | ... | ARGLEN N  | ARG N  |
 *       +-----+-------+----------+----------+-----+-----------+--------+
 *       |  1  |   1   |  1       | Variable | ... | 1         |Variable|
 *       +-----+-------+----------+----------+-----+-----------+--------+
 *
 *   Dónde:
 *
        CMD
           AUTH 0x00
           CLOSE 0x01
           GET 0x02
           SET 0x03
         N-ARGS: indica la cantidad de argumentos .
         ARGLEN i: indica la longitud del argumento i. Como este campo tiene asignado un byte, la longitud máxima de un argumento es 2^8 = 256.
         ARG i: argumento i con longitud ARGLEN i.

 */

/* REQUESTS */
struct hpcp_request_parser {
    /*Request que está siendo parseado*/
   struct hpcp_request *request;
   /*Actual estado del parser*/
   enum hpcp_request_state state;

   /*Cantidad de argumentos*/
   uint8_t nargs;
   /*Argumentos leídos*/
   uint8_t n_read_args;
   /*Bytes leídos*/
   uint8_t n_read_bytes;
   /*Tamaño del argumento que se está leyendo*/
   uint8_t current_arg_size;
   /*Cantidad de bytes leídos del argumento que se está leyendo*/
   uint8_t current_arg_read_bytes;
};

struct hpcp_request {
   //enum  hpcp_req_cmd   cmd;
   // uint8_t*
};

/*Estados en los cuales se puede encontrar el parser*/
enum hpcp_request_state {
   hpcp_request_cmd,
   hpcp_request_nargs,
   hpcp_request_arglen,
   hpcp_request_arg,

   hpcp_request_done,

   //Error
   hpcp_request_error,
   hpcp_request_error_unsupported_version,
   hpcp_request_error_invalid_cmd,
   hpcp_request_error_invalid_args,
   hpcp_request_error_invalid_credentials,
   hpcp_request_error_invalid_transformation_program,
};

//----//

extern enum hpcp_request_state
request_parser_feed (struct hpcp_request_parser* p, const uint8_t c);

static enum hpcp_request_state
cmd_parser(const uint8_t c, struct hpcp_request_parser* p)

#endif











