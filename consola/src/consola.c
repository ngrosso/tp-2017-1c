#include <stdio.h>
#include <stdlib.h>
#include <commons/config.h>

char* ipKernel;
int puertoKernel;
t_config *configConsola;

void inicializarCFG(){
	configConsola=malloc(sizeof(t_config));
	configConsola=config_create("/home/utnso/workspace/consola/src/consola.config");
	ipKernel=config_get_string_value(configConsola,"IP_KERNEL");
	puertoKernel=config_get_int_value(configConsola,"PUERTO_KERNEL");
}

int main(void) {
    inicializarCFG();
    printf("archivo configuracion cargado\n la ip del proceso kernel es: %s puerto: %d \n",ipKernel,puertoKernel);
    return 0;
}

