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
   
    idShMem = shmget(key, sizeof(Controller),  IPC_CREAT | SHM_R | SHM_W);
    controlador = (Controller *)MapearMemoriaComp(idShMem);
    idSem=*((int *)controlador);
    Persona hombre;
    strcpy(hombre.nombre, argv[1]);
    hombre.genero='H';
     enqueue(controlador,hombre);
    display(controlador);
     printf("Servicios higienicos: %d %d\n", *((int *)controlador),idShMem);
     printf("bienvenido %s a los servicios higienicos\n", hombre.nombre);
     DesbloquearSemaforo(idSem, 0);
    for (;;)
    {}
       
}
