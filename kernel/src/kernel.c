#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <commons/config.h>
#include "estructuras.h"


//todo: sacar el ID_CONSOLA de aca, meterlo todo en un solo lugar
//static uint32_t const ID_CONSOLA=1;INICIALIZAR_ANSISOP
//static uint32_t const ID_FILESYSTEM=2;

char* puertoKernel="8080";  //este puerto es provisorio porque puertoKernel no existe
t_config *configKernel;
char* ipMemoria;
char* ipFS;
char* algoritmo;
int puertoProg,puertoCpu,puertoMem,puertoFS,quantum,quantumSleep,gradoMultiprog,stackSize;
char** semIdent;
char** semInit;
char** sharedVars;
void *buf;    // buffer para datos del cliente


void inicializarCFG(){
	configKernel=malloc(sizeof(t_config));
	configKernel=config_create("/home/utnso/tp-2017-1c-ProgramRangers/kernel/src/kernel.config");
	puertoProg=config_get_int_value(configKernel,"PUERTO_PROG");
	puertoCpu=config_get_int_value(configKernel,"PUERTO_CPU");
	ipMemoria=config_get_string_value(configKernel,"IP_MEMORIA");
	puertoMem=config_get_int_value(configKernel,"PUERTO_MEMORIA");
	ipFS=config_get_string_value(configKernel,"IP_FS");
	puertoFS=config_get_int_value(configKernel,"PUERTO_FS");
	quantum=config_get_int_value(configKernel,"QUANTUM");
	quantumSleep=config_get_int_value(configKernel,"QUANTUM_SLEEP");
	algoritmo=config_get_string_value(configKernel,"ALGORITMO");
	gradoMultiprog=config_get_int_value(configKernel,"GRADO_MULTIPROG");
	//ojo, estos 3 array no muestran lo que deberian mostrar HALP
	semIdent=config_get_array_value(configKernel,"SEM_IDS");
	semInit=config_get_array_value(configKernel,"SEM_INIT");
	sharedVars=config_get_array_value(configKernel,"SHARED_VARS");

	stackSize=config_get_int_value(configKernel,"STACK_SIZE");
}

void broadcastMessage(int fdmax, int socket_fd, char msg[256], int nbytes, fd_set* master, int sender);

void inicializarAnsisop(int s_consola, uint32_t cant_leer){
	char* progAnsisop=malloc(cant_leer);
	recv(s_consola,progAnsisop,cant_leer,0);
	printf("Se recibio el siguiente Ansisop:\n");
	printf("%s\n",progAnsisop);
//	inicializarPCB(s_consola,progAnsisop);
	free(progAnsisop);
}


int handShake(int socket) {
	uint32_t idCliente;
//	recvAndDeserialize(socket,&idCliente);
//	int nBytes = &idCliente;
	int nBytes = recv(socket, &idCliente, sizeof(uint32_t), 0);
	if (nBytes <= 0) {
		return 0;
	}
	printf("codigoProceso recibido en handshake %i", idCliente);
	printf("\n");
	switch (idCliente) {
		case ID_CONSOLA:
			return 1;
			break;
//		case ID_FILESYSTEM:
//			return 2;
//			break;
		case ID_MEMORIA:
			return 3;
			break;
		case ID_CPU:
			return 4;
			break;
		default:
			break;
	};
}


void atenderOrdenSegunID(int socket,uint32_t id, int tamanio) {
	uint32_t orden;
	uint32_t tamanio_buffer;
	int mov_puntero=tamanio;
	switch ((int) id) {
		case ID_CONSOLA:
			memcpy(&orden, buf + mov_puntero, sizeof(uint32_t));
			mov_puntero += sizeof(uint32_t);
			switch (orden) {
//			todo: poner esto en una var global a todos los procesos
//			case INICIALIZAR_ANSISOP: {
			case 123: {
					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
					inicializarAnsisop(socket,tamanio_buffer); //aca inicializo el programa Ansisop, osea PCB etc etc
				}
				break;
//				TODO: VER CADA CASO
//				case FINALIZAR_ANSISOP: {
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					buscarPCByEliminar(socket);
//					close(socket);
//					FD_CLR(socket,&master);
//				}
			}
		break;
		case ID_CPU:
			memcpy(&orden, buf + mov_puntero, sizeof(uint32_t));
			mov_puntero += sizeof(uint32_t);
			//				TODO: VER CADA CASO
//			switch((int)orden){
//				case OBTENER_VALOR:
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					obtenerValorGlobal(socket,tamanio_buffer);
//				break;
//				case GRABAR_VALOR:
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					grabarValor(socket,tamanio_buffer);
//				break;
//				case WAIT:
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					waitSemaforo(socket,tamanio_buffer);
//				break;
//				case SIGNAL:
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					signalSemaforo(socket,tamanio_buffer);
//				break;
//				case ENTRADA_SALIDA:
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					valor=actualizarCPUlibre(socket);
//					if(valor==0){
//					bloquearPCBaIO(socket,tamanio_buffer);
//					}
//					else{
//					eliminarDatosNoRelevantesIO(socket,tamanio_buffer);
//					finalizarPCB(socket);
//					//en el metodo finalizarPCB debería obtener el ID del proceso y hacer send a la UMC para que libere la memoria asociada al mismo
//					}
//				break;
//				case PASO_QUANTUM:
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					actualizarQuantumCPU(socket);
//				break;
//				case FIN_ANSISOP:
//					printf("vamo a finalizar el Ansisop\n");
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					finalizarPCBTradicional(socket,tamanio_buffer);
//					//aca debería obtener el ID del proceso y hacer send a la UMC para que libere la memoria asociada al mismo
//				break;
//				case BLOQUEAR_PCB_SEMAFORO:
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					valor=actualizarCPUlibre(socket);
//					if(valor==0){
//					bloquearPCBaSemaforo(socket,tamanio_buffer);
//					}
//					else{
//					eliminarDatosNoRelevantesSem(socket,tamanio_buffer);
//					finalizarPCB(socket);
//					//aca debería obtener el ID del proceso y hacer send a la UMC para que libere la memoria asociada al mismo
//					}
//				break;
//				case IMPRIMIR_TEXTO:
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					mandarAImprimirTexto(socket,tamanio_buffer);
//				break;
//				case FIN_QUANTUM:
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					valor=actualizarCPUlibre(socket);
//					if(valor==0){
//					mandarAready(socket,tamanio_buffer);//tengo que deserializar el pcb y mandarlo a ready
//					}
//					else{
//					finalizarPCBTradicional(socket,tamanio_buffer);
//					//en el metodo finalizarPCB debería obtener el ID del proceso y hacer send a la UMC para que libere la memoria asociada al mismo
//					}
//				break;
//				case FIN_CPU:
//					memcpy(&tamanio_buffer, buf + mov_puntero, sizeof(uint32_t));
//					eliminarCPU(socket);
//				break;
//			}
		break;
	}
	free (buf);
}
//void initProcesoEnMemoria(int pid, int cantPaginas, int socketMemoria){
//	uint32_t resultado;
//	uint32_t procID=pid;
////	uint32_t cantPag=obtenerCantPags(programa);
////	uint32_t tamPrograma=strlen(programa)+1;
//	uint32_t orden=INICIALIZAR_PROGRAMA_MEMORIA;
//	int puntero=0;
//	void* buffer=malloc(sizeof(uint32_t)*4+tamPrograma);
//	memcpy(buffer,&orden,sizeof(uint32_t));
//	puntero+=sizeof(uint32_t);
//	memcpy(buffer+puntero,&procID,sizeof(uint32_t));
//	puntero+=sizeof(uint32_t);
//	memcpy(buffer+puntero,&cantPag,sizeof(uint32_t));
//	puntero+=sizeof(uint32_t);
//	memcpy(buffer+puntero,&tamPrograma,sizeof(uint32_t));
//	puntero+=sizeof(uint32_t);
//	memcpy(buffer+puntero,programa,tamPrograma);
//	puntero+=tamPrograma;
//	send(UMC,buffer,puntero,0);
//	recv(UMC,&resultado,sizeof(uint32_t),0);
//	free(buffer);
//	return resultado;
//}

int main(void) {
    	fd_set master;    // master file descriptor list
    	fd_set read_fds;  // temp file descriptor list for select()
    	int fdmax;        // maximum file descriptor number
    	struct timeval tv;
    	int i, nbytes;
//    	char buf[256];

    	tv.tv_sec = 2;
    	tv.tv_usec = 500000;

    	int status, socket_fd, bindVal, listenVal, new_fd;
    	socklen_t addr_size;
    	struct sockaddr_storage their_addr;
    	struct addrinfo hints, *res;
        inicializarCFG();
        printf("Archivo configuracion cargado\nPuerto para recibir conexiones de programas:%i \nPuerto para recibir conexiones de CPUs:%i \nIP del proceso Memoria:%s puerto:%i \nIP del proceso FileSystem:%s puerto:%i \nQuantum del RoundRobin:%i con retardo de:%i ms \nAlgoritmo(FIFO/RR):%s \nGrado de Multiprogramacion: %i \nTamanio del stack: %i \n",puertoProg,puertoCpu,ipMemoria,puertoMem,ipFS,puertoFS,quantum,quantumSleep,algoritmo,gradoMultiprog,stackSize);

//        todo: aca metemos los inits de variables que se usan en el choclo:
        int valHandshake;
    	FD_ZERO(&master);
    	FD_ZERO(&read_fds);
    	uint32_t idProcesoEntrante;

    	memset(&hints, 0, sizeof hints); // Inicializar el struct
    	hints.ai_family = AF_UNSPEC;
    	hints.ai_socktype = SOCK_STREAM; // Aclaro TCP Stream
    	hints.ai_flags = AI_PASSIVE;

    	if ((status = getaddrinfo(NULL,puertoKernel , &hints, &res)) != 0) { // Inicializar configuracion
    	    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    	    return -1;
    	}

    	socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); // Crear socket
    	if(socket_fd==-1){
    		printf("Error\n");
    		freeaddrinfo(res);
    		return -1;
    	}else{
    		printf("Success in creating socket\n");
    	}

    	bindVal = bind(socket_fd,res->ai_addr,res->ai_addrlen); // Bindear el socket (Reserva el puerto)
    	if(bindVal==-1){
    		fprintf(stderr, "Bind error: %s\n", gai_strerror(bindVal));
    		close(socket_fd);
    		freeaddrinfo(res);
    		return -1;
    	}else{
    		printf("Success in binding port\n");
    	}

    	listenVal = listen(socket_fd,10); // Configura el socket para hacer listener
    	if(listenVal==-1){
    		fprintf(stderr,"Configuring socket to listener : ERROR = %s\n", gai_strerror(listenVal));
    		close(socket_fd);
    		freeaddrinfo(res);
    		return -1;
    	}else{
    		freeaddrinfo(res);
    		printf("Configuring socket to listener : SUCCESS!\n");
    	}
    	FD_SET(socket_fd,&master); // Agrego el socket listener al set
    	fdmax = socket_fd;

    	for(;;){ // CICLO INTERMINAAAAAAAAAAAAAAAAAABLE
    		read_fds = master;
    		if(select(fdmax+1,&read_fds,NULL,NULL,&tv) == -1){
    			perror("Select");
    			return -1;

    		}
    		 for(i = 0; i <= fdmax; i++) { // Recorrer el set de fd
    		            if (FD_ISSET(i, &read_fds)) {
    		            	if(i==socket_fd){ // PETICION DE CONEXION NUEVA
    		            			addr_size = sizeof their_addr;
    		            			new_fd = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);

    		            			if(new_fd==-1){
    		            				fprintf(stderr, "Accept ERROR = %s\n", gai_strerror(new_fd));
    		            				close(socket_fd);
    		            				freeaddrinfo(res);
    		            				return -1;
    		            			}else{
    		            				//handshake sobroson
    		            				valHandshake=handShake(new_fd);
    		            				printf(" valor del handshake: %i \n", valHandshake);
    		            				switch(valHandshake){
    		            				case 1:
    		            					printf("Se conecto la consola\n");
    		            					break;
    		            				case 2:
    		            					printf("Se conecto el filesystem\n");
    		            					break;
    		            				case 3:
    		            					printf("Se conecto la memoria\n");
    		            					break;
    		            				case 4:
    		            					printf("Se conecto el CPU\n");
    		            					break;
    		            				default :
    		            					printf("Error de handshake\n");
    		            					return -1;
    		            				}
    		            				FD_SET(new_fd, &master); //añadir al conjunto maestro
        		            			if (new_fd > fdmax) {   // actualizar el máximo
        		            					fdmax = new_fd;
        		            			}
    		            			}
    		            	}else{ // SI NO ES PETICION DE CONEXION NUEVA
    		    				buf=malloc(sizeof(uint32_t)*3);
    		            		nbytes = recv(i, buf, sizeof(uint32_t)*3, 0);

    		            		if(nbytes  <= 0){ // SI ES ERROR(<0) O DESCONEXION(0)
    		            			if(nbytes == 0){
    		            				printf("Connection of socket N° %i closed\n",i);
    		            			}else if (nbytes < 0){
    		            				printf("Connection error from socket N°%i\n",i);
    		            			}
    		            			close(i);
    		            			FD_CLR(i, &master); // remove from master set
    		            		}
    		            		else{// RE-ENVIAR MENSAJE A TODOS LOS CONECTADOS
    		    					int tamanio=0;
    		    					memcpy(&idProcesoEntrante,buf,sizeof(uint32_t));
    		    					tamanio+=sizeof(uint32_t);
    		    					atenderOrdenSegunID(i,idProcesoEntrante,tamanio);
    		            		}
    		            	}
    		            }
    		 }
    	}
    	close(new_fd);
    	close(socket_fd);
    	FD_ZERO(&master);
    	FD_ZERO(&read_fds);
    	return 0;
}

void broadcastMessage(int fdmax,int socket_fd,char msg[256],int nbytes,fd_set* master, int sender){
	int i;
	for(i = 0; i <= fdmax; i++) {
		if(FD_ISSET(i,master)){
			if(i!= sender && i!= socket_fd){
				if (send(i, msg, nbytes, 0) == -1) {
					fprintf(stderr, "Could not send the message");
				}
			}
		}
	}
}
