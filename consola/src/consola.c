#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>

char ipConsola[10];
int puertoConsola;
t_config *configConsola;

void inicializarCFG(){
	configConsola=malloc(sizeof(t_config));
	configConsola=config_create("/home/utnso/workspace/consola/src/consola.config");
	puertoConsola=config_get_int_value(configConsola,"PUERTO");
}

int main(void) {
    inicializarCFG();
    printf("archivo configuracion cargado\n la ip del consola es:  %d \n",puertoConsola);
    return 0;
}

