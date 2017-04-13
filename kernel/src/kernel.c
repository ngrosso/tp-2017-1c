#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <commons/config.h>


//todo: sacar el ID_CONSOLA de aca, meterlo todo en un solo lugar
static uint32_t const ID_CONSOLA=1;
static uint32_t const ID_FILESYSTEM=2;

int puertoKernel="8080";  //este puerto es provisorio porque puertoKernel no existe
t_config *configKernel;
char* ipMemoria;
char* ipFS;
char* algoritmo;
int puertoProg,puertoCpu,puertoMem,puertoFS,quantum,quantumSleep,gradoMultiprog,stackSize;
char** semIdent;
char** semInit;
char** sharedVars;


void inicializarCFG(){
	configKernel=malloc(sizeof(t_config));
	configKernel=config_create("/home/utnso/git/tp-2017-1c-ProgramRangers/kernel/src/kernel.config");
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



int handShake(int socket) {
	uint32_t idCliente;
	int nBytes = recv(socket, &idCliente, sizeof(uint32_t), 0);
	if (nBytes <= 0) {
		return 0;
	}
	printf("codigoProceso recibido en handshake %i", idCliente);
	printf("\n");
	switch (idCliente) {
	//TODO: aca se van a ir agregando los valores idCliente para cada uno;
	//este mapeo recibo n devuelvo n es para que no se conecte cualquiera
	case 1:
		return 1;
		break;
	case 2:
		return 2;
		break;
	default:
		printf("hubo un error\n");
		return 0;
		break;
	}
}

int main(void) {
    	fd_set master;    // master file descriptor list
    	fd_set read_fds;  // temp file descriptor list for select()
    	int fdmax;        // maximum file descriptor number
    	struct timeval tv;
    	int i, nbytes;
    	char buf[256];

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
    		            				printf(" valor del handshake: %i", valHandshake);
    		            				printf("\n");
    		            				if(valHandshake==1) {
    		            					printf("se conecto la consola\n");
        		            				FD_SET(new_fd, &master); //añadir al conjunto maestro
        		            				if (new_fd > fdmax) {   // actualizar el máximo
        		            					fdmax = new_fd;
        		            				}
    		            				}else if(valHandshake==2) {
    		            					printf("se conecto el filesystem\n");
        		            				FD_SET(new_fd, &master); //añadir al conjunto maestro
        		            				if (new_fd > fdmax) {   // actualizar el máximo
        		            					fdmax = new_fd;
        		            				}
    		            				}
    		            				else{
    		            					printf("conexion invalida\n");
    		            					return 0;
    		            					break;
    		            				}

    		            				//TODO: esto va en cada case
//    		            				FD_SET(new_fd, &master); //añadir al conjunto maestro
//    		            				if (new_fd > fdmax) {   // actualizar el máximo
//    		            					fdmax = new_fd;
//    		            				}
    		            				//fin codigo repetido
    		            			}
    		            	}else{ // SI NO ES PETICION DE CONEXION NUEVA
    		            		nbytes = recv(i, buf, sizeof buf, 0);
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
    		            			printf("el mensaje recibido es: %s \n",buf);
    		            			broadcastMessage(fdmax,socket_fd,buf,nbytes,&master,i); // socket maximo, socket listener, mensaje, length msg, fd_set, sender
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
