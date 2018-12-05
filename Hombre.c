#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "wraper.c"
#include <sys/sem.h>
#include "Structs.c"

int main(int argc, char *argv[])
{
    int idShMem, id; //handle a la memoria compartida
    size_t tabMem;
    int idSem;     //handle al grupo de semáforos
    Controller *controlador;
    key_t key = ftok("shmfile", 65);
   
    idShMem = shmget(key, sizeof(Controller), 0666 | IPC_CREAT);
    controlador = (Controller *)MapearMemoriaComp(idShMem);
    idSem=*((int *)controlador);
    Persona hombre;
    strcpy(hombre.nombre, argv[1]);
    hombre.genero='H';
     enqueue(controlador,hombre);
    // controlador->occupied++;
     printf("Servicios higienicos: %d %d\n", idSem,idShMem);
     printf("bienvenido %s a los servicios higienicos\n", Front(controlador).nombre);
     DesbloquearSemaforo(idSem, 0);
    for (;;)
    {}
       
}
