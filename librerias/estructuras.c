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
#define PCB 3

int serializeAndSend(t_msg* mensaje, int receiver){
	int mallocSize = sizeof(mensaje->m_id) + sizeof(mensaje->m_size) + mensaje->m_size;
	char* aux = malloc(mallocSize);

	memcpy(aux,&mensaje->m_id,sizeof(mensaje->m_id));
	memcpy(aux + sizeof(mensaje->m_id),&mensaje->m_size,sizeof(mensaje->m_size));

	switch(mensaje->m_id){//En caso de ser un struct customizado
	case PCB:
		memcpy(aux + sizeof(mensaje->m_id) + sizeof(mensaje->m_size), &mensaje->m_payload, sizeof(uint32_t)*2);//Al ser 2 int (8bytes, multiplo de 4) no hace falta separar para serializar
		break;
	default:
		memcpy(aux + sizeof(mensaje->m_id) + sizeof(mensaje->m_size), &mensaje->m_payload, &mensaje->m_size);
	}

	send(receiver,aux,mallocSize,0);
	//todo: Send "canchero" (Catcheando excepciones)

	free(aux);
	return 1;
}
int recvAndDeserialize(int sender, void* retrnVal){
	int recvValue;
	int id_buffer;
	int size_buffer;
	char* pl_buffer;
	t_PCB* pcb_buffer = malloc(sizeof(t_PCB));
	recvValue = recv(sender,&id_buffer,4,0);
	if(recvValue == 0) //Si el recv == 0 entonces el socket se desconecto, lo maneja la funcion que llama
		return 0;
	else{
		switch(id_buffer){
		case HANDSHAKE:
			recv(sender,&size_buffer,4,0);
			pl_buffer = malloc(size_buffer);
			recv(sender,&pl_buffer,4,0);
			retrnVal = &pl_buffer;
			break;
		case MENSAJE:
			recv(sender,&size_buffer,4,0);
			pl_buffer = malloc(size_buffer);
			recv(sender,&pl_buffer,size_buffer,0);
			retrnVal = &pl_buffer;
			break;
		case PCB:
			recv(sender,&size_buffer,4,0);
			pl_buffer = malloc(size_buffer);
			recv(sender,&pcb_buffer->PID,sizeof(pcb_buffer->PID),0);
			recv(sender,&pcb_buffer->PC,sizeof(pcb_buffer->PC),0);
			retrnVal = &pcb_buffer;
			break;
		}
	}
	free(pl_buffer);
	free(pcb_buffer);
	return 1;
}
