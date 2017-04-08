#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>

char ipKernel[10];
int puertoKernel;
t_config *configKernel;

void inicializarCFG(){
	configKernel=malloc(sizeof(t_config));
	configKernel=config_create("/home/utnso/tp-2017-1c-ProgramRangers/kernel/src/kernel.config");
	puertoKernel=config_get_int_value(configKernel,"PUERTO");
}

int main(void) {
    inicializarCFG();
    printf("archivo configuracion cargado\n la ip del kernel es:  %d \n",puertoKernel);
    return 0;
}

