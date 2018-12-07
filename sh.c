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
    if (cont->occupied == 0)
    {
        // sprintf (cont->gen,"%d", 1); 
         cont->gen = 'M';
    }
    if (cont->occupied == 0 || cont->gen == Front(cont).genero)
    {
        cont->occupied++;
        printf("-----------------\n");
        printf("Servicios Higienicos con capacidad para %d personas, ocupadas %d \n ", cont->L, cont->occupied);
        printf("ingreso de %s al servicio higienico\n", Front(cont).nombre);
        dequeue(cont);
    }
}
void saleMujer(Controller *cont)
{
}
void entraHombre(Controller *cont)
{
    if (cont->occupied == 0)
    {
// sprintf (cont->gen,"%d", 2); 
        cont->gen = 'H';
    }
    // printf("->%d\n",cont->gen);
    if (cont->occupied == 0 || cont->gen == Front(cont).genero)
    {
        cont->occupied++;
        printf("-----------------\n");
        printf("Servicios Higienicos con capacidad para %d personas, ocupadas %d \n", cont->L, cont->occupied);
        printf("ingreso de %s al servicio higienico\n", Front(cont).nombre);
        dequeue(cont);
    }
    //  printf("->%d\n",cont->gen);
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

    idShMem = shmget(key, sizeof(Controller),  IPC_CREAT | SHM_R | SHM_W);

    controlador = (Controller *)MapearMemoriaComp(idShMem);
    controlador->L = atoi(argv[1]);
    vals[0] = 0;
    vals[1] = 0;
    idSem = CrearSemaforos(9, vals);
    *((int *)controlador) = idSem;

    printf("-----------------\n");
    printf("Servicios Higienicos con capacidad para %d personas, ocupadas %d \n", controlador->L, controlador->occupied);

    while (1)
    {
        *((int *)controlador) = idSem;
        printf("-----------------\n");
        BloquearSemaforo(idSem, 0);
// controlador->gen=0;
        printf("%d",controlador->gen);
        if(controlador->occupied<controlador->L){
            if(Front(controlador).genero=='H'){
                entraHombre(controlador);
            }
            else{
                entraMujer(controlador);
            }
        }        
        
    }

    shmctl(idShMem, IPC_RMID, NULL);

}
