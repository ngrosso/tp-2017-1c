/*
 * estructuras.c
 *
 *  Created on: 17/4/2017
 *      Author: utnso
 */
#include "estructuras.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#define HANDSHAKE 1
#define MENSAJE 2


int serializeAndSend(t_msg* mensaje, int receiver){
	int mallocSize = sizeof(mensaje->m_id) + sizeof(mensaje->m_size) + mensaje->m_size;
	char* aux = malloc(mallocSize);


	memcpy(aux,mensaje,sizeof mensaje);

	send(receiver,aux,mallocSize,0);
	//todo: Send "canchero" (Catcheando excepciones)

	close(aux);
	return 1;
}

int recvAndDeserialize(int sender, void* retrnVal){
	int id_buffer;
	int size_buffer;
	int pl_buffer;
	recv(sender,&id_buffer,4,0);
	switch(id_buffer){
	case HANDSHAKE:
		recv(sender,NULL,4,0);
		recv(sender,&pl_buffer,4,0);
		retrnVal = &pl_buffer;
		break;
	case MENSAJE:
		recv(sender,&size_buffer,4,0);
		char pl_buffer[size_buffer];
		recv(sender,pl_buffer,size_buffer,0);
		retrnVal = &pl_buffer;
		break;
	}
	return 1;
}
