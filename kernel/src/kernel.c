#include <stdio.h>
#include <stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include <commons/config.h>

char ipKernel[10];
char* puertoKernel;
t_config *configKernel;

void inicializarCFG(){
	configKernel=malloc(sizeof(t_config));
	configKernel=config_create("/home/utnso/git/tp-2017-1c-ProgramRangers/kernel/src/kernel.config");
	puertoKernel=config_get_string_value(configKernel,"PUERTO");
}

void broadcastMessage(int fdmax, int socket_fd, char msg[256], int nbytes, fd_set* master, int sender);

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
        printf("Archivo configuracion cargado\n La ip del kernel es:  %s \n",puertoKernel);


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
    		            				FD_SET(new_fd, &master);
    		            				if (new_fd > fdmax) {   // MANTENER EL MAX
    		            					fdmax = new_fd;
    		            				}
    		            				printf("Accept SUCCESS!\n");
    		            			}
    		            	}else{ // SI NO ES PETICION DE CONEXION NUEVA
    		            		nbytes = recv(i, buf, sizeof buf, 0);
    		            		if(nbytes  <= 0){ // SI ES ERROR(<0) O DESCONEXION(0)
    		            			if(nbytes == 0){
    		            				printf("Connection of socket N° %i closed",i);
    		            			}else if (nbytes < 0){
    		            				printf("Connection error from socket N°%i",i);
    		            			}
    		            			close(i);
    		            			FD_CLR(i, &master); // remove from master set
    		            		}
    		            		else{// RE-ENVIAR MENSAJE A TODOS LOS CONECTADOS
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
