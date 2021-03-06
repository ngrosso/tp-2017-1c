/*
 * estructuras.h
 *
 *  Created on: 17/4/2017
 *      Author: utnso
 */

#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <commons/collections/list.h>
#include <stdint.h>
#include <parser/metadata_program.h>

typedef enum {
	ID_KERNEL,
	ID_CPU,
	ID_CONSOLA,
	ID_MEMORIA,
	ID_FILESYSTEM,
	INICIALIZAR_PROGRAMA_MEMORIA, //El Kernel indica a memoria que inicia un programa.
	FINALIZAR_PROGRAMA_MEMORIA,
	INIT_ANSISOP,//El Kernel indica a memoria que termino el programa, tambien puede usarlo la UMC para avisar.
} identificadores;

typedef struct {
	uint32_t PID;
	uint32_t PC;
	uint32_t program_counter;
	t_intructions  indice_codigo[];
} t_PCB;

typedef struct { // Struct de headers de envio de datos
	uint32_t m_id;
	uint32_t m_size;
	void* m_payload;
} t_msg;

int serializeAndSend(t_msg* mensaje, int receiver);

int recvAndDeserialize(int sender,void* retrnVal);

#endif /* ESTRUCTURAS_H_ */
