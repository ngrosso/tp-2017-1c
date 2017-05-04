#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include "estructuras.h"

//GLOBALES
int variablemagica = 0;
char* ipKernel;
int puertoKernel;
t_config *configConsola;
//todo: sacar el ID_CONSOLA de aca, meterlo todo en un solo lugar, podemos hacerlo con un enum
//static uint32_t const ID_CONSOLA=1;
int server;
FILE* progAnsisop;
void* buffer;


//TODO: ESTO TIENE QUE IR A UNA LIBRERIA
void crearCliente(struct sockaddr_in* direccionServidor, int puerto, char* ip) {
	direccionServidor->sin_family = AF_INET;
	direccionServidor->sin_addr.s_addr = inet_addr(ip);
	direccionServidor->sin_port = htons(puerto);
}

void enviarAnsisop(int server, char* argumento){
	progAnsisop=fopen(argumento,"rb");
	if (progAnsisop == NULL){
		printf("No se pudo abrir el archivo\n");
		abort();
	}

	fseek(progAnsisop,0,SEEK_END);
	//	uint32_t sizeAnsisop=(ftell(progAnsisop)-115);
	//	fseek(progAnsisop,115,SEEK_SET);
	uint32_t sizeAnsisop=ftell(progAnsisop);
	fseek(progAnsisop,0,SEEK_SET);
	void*bufferArchivo=malloc(sizeAnsisop);
	fread(bufferArchivo,sizeAnsisop,1,progAnsisop);
	buffer=malloc(sizeAnsisop+(sizeof(uint32_t)*3));
	printf("%s\n",(char*)bufferArchivo);
	uint32_t idconsola= ID_CONSOLA;
//	TODO: PONER LA ORDEN
//	uint32_t orden = INIT_ANSISOP;
	uint32_t orden = 123;
	memcpy(buffer,&idconsola,sizeof(uint32_t));
	memcpy(buffer+sizeof(uint32_t),&orden,sizeof(uint32_t));
	memcpy(buffer+(sizeof(uint32_t)*2),&sizeAnsisop,sizeof(uint32_t));
	memcpy(buffer+(sizeof(uint32_t)*3),bufferArchivo,sizeAnsisop);
	send(server,buffer,((sizeof(uint32_t)*3)+sizeAnsisop),0);
	free(bufferArchivo);
}

void inicializarCFG() {
	configConsola = malloc(sizeof(t_config));
	configConsola =
			config_create(
					"/home/utnso/tp-2017-1c-ProgramRangers/consola/src/consola.config");
	ipKernel = config_get_string_value(configConsola, "IP_KERNEL");
	puertoKernel = config_get_int_value(configConsola, "PUERTO_KERNEL");
}

void handshake(int socket) {
	uint32_t idCliente = ID_CONSOLA;
	send(socket,&idCliente,sizeof(uint32_t),0);

//	t_msg mensaje;
//	mensaje.m_id = 1;
//	mensaje.m_size = sizeof(uint32_t);
//	mensaje.m_payload = &idCliente;
//	serializeAndSend(&mensaje, socket);
}

void consoleToKernel(int socket) {
	char buf[256];
	scanf("%s", buf);
	serializeAndSend(&buf, socket);
}

void clearScreen() {
	const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
	write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void hiloPrograma(){
//	TODO: unimplemented method;
}

		int main(void) {

			inicializarCFG();
			printf(
					"archivo configuracion cargado\n la ip del proceso kernel es: %s puerto: %d \n",
					ipKernel, puertoKernel);


			//Conexion con el Kernel, podria ir en la libreria tambien(no nos emocionemos)-------------------------------------------------------------------------------------------------------------------
			struct sockaddr_in direccionServidor;
			server = socket(AF_INET, SOCK_STREAM, 0);
			crearCliente(&direccionServidor, puertoKernel, ipKernel);

			if (connect(server, (void*) &direccionServidor,
					sizeof(direccionServidor)) != 0) {
				perror("No se pudo conectar al kernel");
				return 1;
			}

			//Verifico conexion con el Kernel-----------------------------------------------------------------------------------------------------------
			handshake(server);

//			consoleToKernel(server);
			int seleccion;
			char *bufferAnsisop=malloc(256);
			while (1) {
				puts("Seleccione una opcion: \n1 para Iniciar Programa \n2 para Finalizar Programa \n3 para Desconectar la consola \n4 para Limpiar consola\n5 para enviar Ansisop");
				scanf("%i", &seleccion);
				switch (seleccion) {
				case 1:
					printf("Inciar programa\n\n");
					pthread_t hiloPrograma;
								pthread_attr_t atributo;
								pthread_attr_init(&atributo);
								pthread_attr_setdetachstate(&atributo, PTHREAD_CREATE_DETACHED);
								pthread_create(&hiloPrograma, &atributo, (void*)hiloPrograma, NULL);
					break;
				case 2:
					printf("Finalizar Programa\n\n");

					break;
				case 3:
					printf("Desconectar Consola\n\n");

					break;
				case 4:
					printf("Limpiar Consola\n\n");
					clearScreen();
					printf("\n");
					printf("\n");
					break;
				case 5:
					printf("Ingresar Path\n");
					scanf("%s", bufferAnsisop);
					enviarAnsisop(server, bufferAnsisop);
					break;
				default:
					printf("Ingrese una opcion valida\n\n");
				}
			}
		return 0;
}

