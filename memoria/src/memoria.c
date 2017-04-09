#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>

char* reemplazoCache;
int puerto,marcos,tamanioMarco,entradasCache,maxEntradasCacheXProg,retardoMemoria;
t_config *configMemoria;

void inicializarCFG(){
	configMemoria=malloc(sizeof(t_config));
	configMemoria=config_create("/home/utnso/git/tp-2017-1c-ProgramRangers/memoria/src/memoria.config");
	puerto=config_get_int_value(configMemoria,"PUERTO");
	marcos=config_get_int_value(configMemoria,"MARCOS");
	tamanioMarco=config_get_int_value(configMemoria,"MARCO_SIZE");
	entradasCache=config_get_int_value(configMemoria,"ENTRADAS_CACHE");
	maxEntradasCacheXProg=config_get_int_value(configMemoria,"CACHE_X_PROC");
	reemplazoCache=config_get_string_value(configMemoria,"REEMPLAZO_CACHE");
	retardoMemoria=config_get_int_value(configMemoria,"RETARDO_MEMORIA");
}

int main(void) {
    inicializarCFG();
    printf("archivo configuracion cargado\nPuerto para recibir conexiones del Kernel y CPUs: %i \nCantidad marcos disponibles: %i \nTamanio de marcos(en bytes): %i \nCantidad entradas en cache(0=deshabilitada): %i \nEntradas asignables a la cache max: %i \nReemplazo cache: %s \nRetardo de memoria:%i \n",puerto,marcos,tamanioMarco,entradasCache,maxEntradasCacheXProg,reemplazoCache,retardoMemoria);
    return 0;
}

