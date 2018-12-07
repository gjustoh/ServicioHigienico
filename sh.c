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
        printf("**********************************************************************\n\n");
        printf("capacidad para %d personas, ocupadas %d \n ", cont->L, cont->occupied);
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
        printf("**********************************************************************\n\n");

        printf("ingreso de %s al servicio higienico %d\n", Front(cont).nombre, Front(cont).id);

        printf("baños ocupados  %d de %d\n\n", cont->occupied, cont->L);

        dequeue(cont);
        printf("cola de espera:\n");
        display(cont);
        DesbloquearSemaforo(cont->id_proceso_2, Front(cont).id);
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
    int idShMem, id;
    size_t tabMem;
    key_t llave, key;
    int idSem, idSem2;
    char *buf;
    short vals[2];
    int miSem;
    int tuSem;
    Controller *controlador;
    key = ftok("shmfile", 65);
    llave = ftok("shmf", 33);

    idShMem = shmget(key, sizeof(Controller), IPC_CREAT | SHM_R | SHM_W);

    controlador = (Controller *)MapearMemoriaComp(idShMem);
    controlador->L = atoi(argv[1]);

    idSem = CrearSemaforos(9, vals);
    idSem2 = CrearSemaforos(9, vals);
    controlador->id_proceso_2 = idSem2;
    *((int *)controlador) = idSem;

    printf("**********************************************************************\n");
    printf("**********************************************************************\n");
    printf("*************CONTROLADOR DE SERVICIOS HIGIENICOS**********************\n");
    printf("**********************************************************************\n");
    printf("**********************************************************************\n\n");
    printf("capacidad para %d personas, ocupadas %d \n", controlador->L, controlador->occupied);

    while (1)
    {
        *((int *)controlador) = idSem;

        BloquearSemaforo(idSem, 0);
        // controlador->gen=0;
        // printf("%d", controlador->gen);
        if (controlador->occupied < controlador->L)
        {
            if (Front(controlador).genero == 'H')
            {
                entraHombre(controlador);
            }
            else
            {
                entraMujer(controlador);
            }
        }
        else
        {
            printf("**********************************************************************\n\n");
            printf("Servicios higienicos llenos, espere hasta que se desocupe uno \n\n");
            printf("cola de espera:\n");
            display(controlador);
            printf("**********************************************************************\n\n");
        }
    }

    shmctl(idShMem, IPC_RMID, NULL);
}
