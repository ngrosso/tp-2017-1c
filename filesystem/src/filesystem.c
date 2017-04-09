#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>

char* puntoMontaje;
int puerto;
t_config *configFS;

void inicializarCFG(){
	configFS=malloc(sizeof(t_config));
	configFS=config_create("/home/utnso/git/tp-2017-1c-ProgramRangers/filesystem/src/filesystem.config");
	puerto=config_get_int_value(configFS,"PUERTO");
	puntoMontaje=config_get_string_value(configFS,"PUNTO_MONTAJE"); //no muestra la ruta
}

int main(void) {
    inicializarCFG();
    printf("archivo configuracion cargado\nPuerto para recibir conexiones del Kernel: %i \nPunto de montaje inicial: %s \n",puerto,puntoMontaje);
    return 0;
}

