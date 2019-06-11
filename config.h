/**
 * Este archivo declara la funcionalidad de las funciones utilizadas
 * para configurar el proxy
 * */

#include <stdbool.h>
#include <unitypes.h>

/**
 * Estructura para guardar configuraciones del proxy
 * */
typedef struct {

} proxy_configs;

/**
 * Variable para guardar las configuraciones e importarla desde otros source files
 * Recomendaciones:
 * https://stackoverflow.com/questions/1433204/how-do-i-use-extern-to-share-variables-between-source-files
 * A header file only contains extern declarations of variables — never static or unqualified variable definitions.
 * For any given variable, only one header file declares it (SPOT — Single Point of Truth).
 * A source file never contains extern declarations of variables — source files always include the (sole) header that declares them.
 * For any given variable, exactly one source file defines the variable, preferably initializing it too. (Although there is no need to initialize explicitly to zero, it does no harm and can do some good, because there can be only one initialized definition of a particular global variable in a program).
 * The source file that defines the variable also includes the header to ensure that the definition and the declaration are consistent.
 * A function should never need to declare a variable using extern
 * */
extern proxy_configs global_variable;

// Estructura de maite todo borrar del archivo de maite
/**
 * Estructura y enum para usar de pasa manos entre quien procesa el request
 * y quien lleva a cabo la configuración
 * */
typedef enum hpcp_req_cmd {
    HELLO  = 0x00,
    AUTH   = 0x01,
    CLOSE  = 0x02,
    GET    = 0x03,
    SET    = 0x04,
    ERR    = -0x01,
} hpcp_req_cmd;

typedef struct {
    enum hpcp_req_cmd cmd;
    uint8_t **  args;
    size_t *    arg_sizes;
    size_t      nargs;
} hpcp_request;

/**
 * Función que recibe los parametros del request procesados.
 * Asume que los métodos del request son válidos.
 * Pero no asume que los parametros son válidos
 * */
hpcp_request *config(hpcp_request *req);

/**
 * Los siguientes son handlers basados en el cmd
 * */
void handle_hello(hpcp_request *req, hpcp_request *res);
void handle_auth(hpcp_request *req, hpcp_request *res);
void handle_close(hpcp_request *req, hpcp_request *res);
void handle_get(hpcp_request *req, hpcp_request *res);
void handle_set(hpcp_request *req, hpcp_request *res);