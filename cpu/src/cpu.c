#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netdb.h>

int server;
static uint32_t const ID_CPU=4;

//TODO: ESTO TIENE QUE IR A UNA LIBRERIA
void crearCliente(struct sockaddr_in* direccionServidor, int puerto, char* ip) {
	direccionServidor->sin_family = AF_INET;
	direccionServidor->sin_addr.s_addr = inet_addr(ip);
	direccionServidor->sin_port = htons(puerto);
}

void handshake(int socket) {
	uint32_t idCliente=ID_CPU;
	send(socket,&idCliente,sizeof(uint32_t),0);
	send(socket,"holiwis",8,0);
}

int main(void) {
	struct sockaddr_in direccionServidor;
	server = socket(AF_INET, SOCK_STREAM, 0);
	crearCliente(&direccionServidor,8080,"127.0.0.1");
	if (connect(server, (void*) &direccionServidor, sizeof(direccionServidor))!= 0) {
		perror("No se pudo conectar al kernel");
	    return 1;
	}
	handshake(server);
	return 0;

}
