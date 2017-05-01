#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <parser/parser.h>
#include <estructuras.h>
#include <estructuras.c>

int serverKernel,serverMemoria,puertoKernel,puertoMemoria;
char *ipKernel;


char *ipMemoria;
char buf[256];
AnSISOP_funciones memFunc;
AnSISOP_kernel kerFunc;
t_config *configCpu;


//TODO: ESTO TIENE QUE IR A UNA LIBRERIA
	void crearCliente(struct sockaddr_in* direccionServidor, int puerto, char* ip) {
	direccionServidor->sin_family = AF_INET;
	direccionServidor->sin_addr.s_addr = inet_addr(ip);
	direccionServidor->sin_port = htons(puerto);
}
void initCpuCFG(){
	configCpu=malloc(sizeof(t_config));
	configCpu=config_create("/home/utnso/tp-2017-1c-ProgramRangers/cpu/src/cpu.config");
	ipKernel=config_get_int_value(configCpu,"IP_KERNEL");
	ipMemoria=config_get_int_value(configCpu,"IP_MEMORIA");
	puertoKernel=config_get_int_value(configCpu,"PUERTO_KERNEL");
	puertoMemoria=config_get_int_value(configCpu,"PUERTO_MEMORIA");
}

/* memory functions*/

t_puntero AnSISOP_definirVariable(t_nombre_variable identificador_variable){
	int a=3;
	t_puntero i = &a;
	printf("se invoca definir variable");
	return i;
}

t_puntero AnSISOP_obtenerPosicionVariable(t_nombre_variable identificador_variable){
	t_puntero i;
	printf("se invoca pocision variable");
	return i;
}

t_valor_variable AnSISOP_dereferenciar(t_puntero direccion_variable){
	t_valor_variable i;
	printf("se invoca deferenciar variable valor:%d\n",direccion_variable);
	return i;
}

void AnSISOP_asignar(t_puntero direccion_variable, t_valor_variable valor)
{
	direccion_variable*= valor;
	printf("se invoca direccion variable");
}

t_valor_variable AnSISOP_obtenerValorCompartida(t_nombre_compartida variable){
	t_valor_variable i;
	printf("se invoca obtener valor compartida");
	return i;
}

/* fin memory functions*/

/* kernel functions*/
void AnSISOP_wait(t_nombre_semaforo identificador_semaforo){
	printf("se invoca kernel wait");
}

void AnSISOP_signal(t_nombre_semaforo identificador_semaforo){
	printf("se invoca kernel signal");
}

t_puntero AnSISOP_reservar(t_valor_variable espacio){
	t_puntero i;
	printf("se invoca kernel pocision variable");
	return i;
}
/* fin kernel functions*/

void asignarFunciones(){
	memFunc.AnSISOP_definirVariable = AnSISOP_definirVariable;
	memFunc.AnSISOP_obtenerPosicionVariable = AnSISOP_obtenerPosicionVariable;
	memFunc.AnSISOP_asignar = AnSISOP_asignar;
	memFunc.AnSISOP_obtenerValorCompartida = AnSISOP_obtenerValorCompartida;
	kerFunc.AnSISOP_wait = AnSISOP_wait;
	kerFunc.AnSISOP_signal = AnSISOP_signal;
	kerFunc.AnSISOP_reservar = AnSISOP_reservar;
}

void handshake(int socket) {
	uint32_t idCliente=ID_CPU;
	send(socket,&idCliente,sizeof(uint32_t),0);
	recv(socket,buf,8,0);
	printf("el mensaje recibido del kernel es: %s \n",buf);
}

void parsearLinea(char *linea){
	analizadorLinea(linea,&memFunc,&kerFunc);
}

int kernelListener (){
	struct sockaddr_in direccionKernelServidor;
	serverKernel = socket(AF_INET, SOCK_STREAM, 0);
	crearCliente(&direccionKernelServidor,puertoKernel,ipKernel);
	if (connect(
	serverKernel, (void*) &direccionKernelServidor, sizeof(direccionKernelServidor))!= 0) {
		perror("No se pudo conectar al Kernel");
	    return 1;
	}
	handshake(serverKernel);
	while(1){
		recv(serverKernel,buf,8,0);

	}
}

int connectToMemory(){
	struct sockaddr_in direccionMemoryServidor;
	serverMemoria = socket(AF_INET, SOCK_STREAM, 0);
	crearCliente(&direccionMemoryServidor,puertoMemoria,ipMemoria);
	if (connect(serverMemoria, (void*) &direccionMemoryServidor, sizeof(direccionMemoryServidor))!= 0) {
		perror("No se pudo conectar al memoria");
	    return 1;
	}
	handshake(serverMemoria);
	return 0;
}


int main(void) {
	asignarFunciones();
	parsearLinea("variables a");
	return 0;

}
