#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netdb.h>

char* reemplazoCache;
int puerto,marcos,tamanioMarco,entradasCache,maxEntradasCacheXProg,retardoMemoria,server;
t_config *configMemoria;
static uint32_t const ID_MEMORIA=3;
char buf[256];

//TODO: ESTO TIENE QUE IR A UNA LIBRERIA
void crearCliente(struct sockaddr_in* direccionServidor, int puerto, char* ip) {
	direccionServidor->sin_family = AF_INET;
	direccionServidor->sin_addr.s_addr = inet_addr(ip);
	direccionServidor->sin_port = htons(puerto);
}

void inicializarCFG(){
	configMemoria=malloc(sizeof(t_config));
	configMemoria=config_create("/home/utnso/git/tp-2017-1c-ProgramRangers/memoria/src/memoria.config");
	puerto=config_get_int_value(configMemoria,"PUERTO");
	marcos=config_get_int_value(configMemoria,"MARCOS");
	tamanioMarco=config_get_int_value(configMemoria,"MARCO_SIZE");
	entradasCache=config_get_int_value(configMemoria,"ENTRADAS_CACHE");
	maxEntradasCacheXProg=config_get_int_value(configMemoria,"CACHE_X_PROC");
	reemplazoCache=config_get_string_value(configMemoria,"REEMPLAZO_CACHE");
	retardoMemoria=config_get_int_value(configMemoria,"RETARDO_MEMORIA");
}

void handshake(int socket) {
	uint32_t idCliente=ID_MEMORIA;
	send(socket,&idCliente,sizeof(uint32_t),0);
	recv(socket,buf,8,0);
	printf("el mensaje recibido del kernel es: %s \n",buf);
}

int main(void) {
    inicializarCFG();
    printf("archivo configuracion cargado\nPuerto para recibir conexiones del Kernel y CPUs: %i \nCantidad marcos disponibles: %i \nTamanio de marcos(en bytes): %i \nCantidad entradas en cache(0=deshabilitada): %i \nEntradas asignables a la cache max: %i \nReemplazo cache: %s \nRetardo de memoria:%i \n",puerto,marcos,tamanioMarco,entradasCache,maxEntradasCacheXProg,reemplazoCache,retardoMemoria);

    //HARDCODED
    struct sockaddr_in direccionServidor;
    	server = socket(AF_INET, SOCK_STREAM, 0);
    	crearCliente(&direccionServidor,8080,"127.0.0.1");
    	if (connect(server, (void*) &direccionServidor, sizeof(direccionServidor))
    			!= 0) {
    		perror("No se pudo conectar al kernel");
    		return 1;
    	}

    handshake(server);

    return 0;
}

