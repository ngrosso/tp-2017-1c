/*
 * estructuras.c
 *
 *  Created on: 17/4/2017
 *      Author: utnso
 */
#include "estructuras.h"

#include <stdlib.h>



void serializeAndSend(t_msg* mensaje, int receiver){
	int mallocSize = sizeof(mensaje->m_id) + sizeof(mensaje->m_size) + mensaje->m_size;
	char* aux = malloc(mallocSize);

	memcpy(aux,&mensaje->m_id,sizeof mensaje->m_id);
	memcpy(aux,&mensaje->m_size,sizeof mensaje->m_size);
	memcpy(aux,&mensaje->m_payload,sizeof mensaje->m_payload);

	// TO DO: SEND

	close(aux);
}
