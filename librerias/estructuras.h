/*
 * estructuras.h
 *
 *  Created on: 17/4/2017
 *      Author: utnso
 */

#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_


typedef enum {
	ID_KERNEL,
	ID_CPU,
	ID_CONSOLA,
	ID_MEMORIA,
	INICIALIZAR_PROGRAMA_MEMORIA, //El Kernel indica a memoria que inicia un programa.
	FINALIZAR_PROGRAMA_MEMORIA, //El Kernel indica a memoria que termino el programa, tambien puede usarlo la UMC para avisar.
							//al nucleo que termine un programa(Se puede usar cuando no hay frames para realizar el algoritmo de clock, que conlleva a matar el proceso)
} identificadores;


#endif /* ESTRUCTURAS_H_ */
