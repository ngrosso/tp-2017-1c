#include <commons/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <estructuras.h>
#include <estructuras.c>
#include <pthread.h>
#include <unistd.h>
#define ID_CONSOLA 1

//GLOBALES
int variablemagica = 0;
char* ipKernel;
int puertoKernel;
t_config *configConsola;
//todo: sacar el ID_CONSOLA de aca, meterlo todo en un solo lugar, podemos hacerlo con un enum
//static uint32_t const ID_CONSOLA=1;
int server;

//TODO: ESTO TIENE QUE IR A UNA LIBRERIA
void crearCliente(struct sockaddr_in* direccionServidor, int puerto, char* ip) {
	direccionServidor->sin_family = AF_INET;
	direccionServidor->sin_addr.s_addr = inet_addr(ip);
	direccionServidor->sin_port = htons(puerto);
}

void inicializarCFG() {
	configConsola = malloc(sizeof(t_config));
	configConsola =
			config_create(
					"/home/utnso/git/tp-2017-1c-ProgramRangers/consola/src/consola.config");
	ipKernel = config_get_string_value(configConsola, "IP_KERNEL");
	puertoKernel = config_get_int_value(configConsola, "PUERTO_KERNEL");
}

void handshake(int socket) {
	uint32_t idCliente = ID_CONSOLA;
	t_msg mensaje;
	mensaje.m_id = 1;
	mensaje.m_size = sizeof(uint32_t);
	mensaje.m_payload = &idCliente;
	serializeAndSend(&mensaje, socket);
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

int hiloPrograma(){
	return 1;
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

			consoleToKernel(server);
			while (1) {
				puts("Seleccione una opcion: \n1 para Iniciar Programa \n2 para Finalizar Programa \n3 para Desconectar la consola \n4 para Limpiar consola\n");
				int seleccion;
				scanf("%i", &seleccion);
				switch (seleccion) {
				case 1:
					printf("Inciar programa\n\n");
					pthread_t hiloPrograma;
								pthread_attr_t atributo;
								pthread_attr_init(&atributo);
								pthread_attr_setdetachstate(&atributo, PTHREAD_CREATE_DETACHED);
								pthread_create(&hiloPrograma, &atributo, (int*)hiloPrograma, NULL);
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
				default:
					printf("Ingrese una opcion valida\n\n");
				}
			}
		return 0;
}

