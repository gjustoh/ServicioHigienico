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

void entraMujer(Controller *cont)
{
    cont->val_gen = 'H';
}
void saleMujer(Controller *cont)
{
}
void entraHombre(Controller *cont)
{
}
void saleHombre(Controller *cont)
{
}
int isComplet(Controller *cont)
{
    return cont->L == cont->occupied;
}
int isEmpty(Controller *cont)
{
    return cont->occupied == 0;
}

int main(int argc, char *argv[])
{
    int idShMem, id; //handle a la memoria compartida
    size_t tabMem;
    key_t llave, key;
    int idSem;     //handle al grupo de semáforos
    char *buf;     //dirección del buffer de memoria compartida
    short vals[2]; //valores para inicializar los semáforos
    int miSem;     //semáforo para nuestro usuario
    int tuSem;     //semáforo para el otro usuario
    Controller *controlador;
    key = ftok("shmfile", 65);
    llave = ftok("shmf", 33);

    //no nos pasaron ningún id, así que lo creamos

    idShMem = shmget(key, sizeof(Controller), 0666 | IPC_CREAT);

    controlador = (Controller *)MapearMemoriaComp(idShMem);
    controlador->L = atoi(argv[1]);
    vals[0] = 0;
    vals[1] = 0;
    idSem = CrearSemaforos(2, vals);
    *((int *)controlador) = idSem;

    printf("-----------------\n");
    printf("Servicios Higienicos con capacidad para %d personas, ocupadas %d \n", controlador->L, controlador->occupied);
    BloquearSemaforo(idSem, 0);

    if (controlador->occupied == 0 || controlador->val_gen == Front(controlador).genero)
    {
        controlador->occupied++;
        printf("-----------------\n");
        printf("Servicios Higienicos con capacidad para %d personas, ocupadas %d \n", controlador->L, controlador->occupied);

        printf("ingreso de %s al servicio higienico\n", Front(controlador).nombre);
        controlador->occupied++;
    }
    //  while(1){
    // // printf("-%d - %d\n", *((int *)controlador), idSem);
    // BloquearSemaforo(idSem, 0);
    // printf("-----------------\n");
    // printf("Servicios higienicos - capacidad para %d personas, ocupadas %d\n", controlador->L, controlador->occupied);

    // display(controlador);
    // // sleep(5);

    // }
    shmctl(idShMem, IPC_RMID, NULL);

    // printf("Eres el usuario uno. El id de la memoria compartida es: %d\n", idShMem);
}
