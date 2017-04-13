#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

char* puntoMontaje;
int puerto;
t_config *configFS;
int server;
char* ipKernel;
int puertoKernel;
//todo: sacar el ID_FS de aca, meterlo todo en un solo lugar
//podemos hacerlo con un enum?? secundario
static uint32_t const ID_FS=2;
char buf[256];


//TODO: ESTO TIENE QUE IR A UNA LIBRERIA
void crearCliente(struct sockaddr_in* direccionServidor, int puerto, char* ip) {
	direccionServidor->sin_family = AF_INET;
	direccionServidor->sin_addr.s_addr = inet_addr(ip);
	direccionServidor->sin_port = htons(puerto);
}

void inicializarCFG(){
	configFS=malloc(sizeof(t_config));
	configFS=config_create("/home/utnso/git/tp-2017-1c-ProgramRangers/filesystem/src/filesystem.config");
	puerto=config_get_int_value(configFS,"PUERTO");
	puntoMontaje=config_get_string_value(configFS,"PUNTO_MONTAJE"); //no muestra la ruta
	ipKernel=config_get_string_value(configFS,"IP_KERNEL");
	puertoKernel=config_get_int_value(configFS,"PUERTO_KERNEL");
}

void handshake(int socket) {
	uint32_t idCliente=ID_FS;
	send(socket,&idCliente,sizeof(uint32_t),0);
	recv(socket,buf,8,0);
	printf("el mensaje recibido del kernel es: %s \n",buf);
}

int main(void) {
    inicializarCFG();
    printf("archivo configuracion cargado\nPuerto para recibir conexiones del Kernel: %i \nPunto de montaje inicial: %s \n",puerto,puntoMontaje);

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

    return 0;
}

