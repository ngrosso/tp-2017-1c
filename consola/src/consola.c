#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

//GLOBALES
char* ipKernel;
int puertoKernel;
t_config *configConsola;
//todo: sacar el ID_CONSOLA de aca, meterlo todo en un solo lugar, podemos hacerlo con un enum
static uint32_t const ID_CONSOLA=1;
int server;

//TODO: ESTO TIENE QUE IR A UNA LIBRERIA
void crearCliente(struct sockaddr_in* direccionServidor, int puerto, char* ip) {
	direccionServidor->sin_family = AF_INET;
	direccionServidor->sin_addr.s_addr = inet_addr(ip);
	direccionServidor->sin_port = htons(puerto);
}

void inicializarCFG(){
	configConsola=malloc(sizeof(t_config));
	configConsola=config_create("/home/utnso/git/tp-2017-1c-ProgramRangers/consola/src/consola.config");
	ipKernel=config_get_string_value(configConsola,"IP_KERNEL");
	puertoKernel=config_get_int_value(configConsola,"PUERTO_KERNEL");
}

void handshake(int socket) {
	uint32_t idCliente=ID_CONSOLA;
	send(socket,&idCliente,sizeof(uint32_t),0);
}

void consoleToKernel(int socket){
	char buf[256];
	scanf("%s",buf);
	send(socket,buf,strlen(buf),0);
}

int main(void) {
    inicializarCFG();
    printf("archivo configuracion cargado\n la ip del proceso kernel es: %s puerto: %d \n",ipKernel,puertoKernel);

	//Conexion con el Kernel, podria ir en la libreria tambien(no nos emocionemos)-------------------------------------------------------------------------------------------------------------------
	struct sockaddr_in direccionServidor;
	server = socket(AF_INET, SOCK_STREAM, 0);
	crearCliente(&direccionServidor,puertoKernel,ipKernel);
	if (connect(server, (void*) &direccionServidor, sizeof(direccionServidor))
			!= 0) {
		perror("No se pudo conectar al kernel");
		return 1;
	}

	//Verifico conexion con el Kernel-----------------------------------------------------------------------------------------------------------
	handshake(server);

	consoleToKernel(server);

    return 0;
}

