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
        cont->gen = 'M';
    }
    if (cont->occupied == 0 || cont->gen == Front(cont).genero)
    {
        cont->occupied++;
        printf("**********************************************************************\n\n");
        printf("ingreso de %s al servicio higienico %d\n", Front(cont).nombre, Front(cont).id);
        printf("baños ocupados  %d de %d\n\n", cont->occupied, cont->L);
        printf("cola de espera:\n");
        display2(cont);
        printf("\n\n");
        DesbloquearSemaforo(cont->id_proceso_2, Front(cont).id);
        dequeue(cont);
    }
    else
    {
        printf("**********************************************************************\n\n");
        printf("%s en la cabeza esperando que salgan los hombres:\n\n", Front(cont).nombre);
        // printf("**********************************************************************\n\n");
    }
}

void entraHombre(Controller *cont)
{
    if (cont->occupied == 0)
    {
        cont->gen = 'H';
    }
    if (cont->occupied == 0 || cont->gen == Front(cont).genero)
    {
        cont->occupied++;
        printf("**********************************************************************\n\n");

        printf("ingreso de %s al servicio higienico %d\n", Front(cont).nombre, Front(cont).id);

        printf("baños ocupados  %d de %d\n\n", cont->occupied, cont->L);
        printf("cola de espera:\n");
        display2(cont);
        printf("\n\n");
        DesbloquearSemaforo(cont->id_proceso_2, Front(cont).id);
        
        dequeue(cont);
    }
    else
    {
        printf("**********************************************************************\n\n");
        printf("%s en la cabeza esperando que salgan los hombres:\n\n", Front(cont).nombre);
    }
}
void saleMujer(Controller *cont, int id)
{
    printf("Mujer %s saliendo del servicio higienico\n\n",cont->per[id].nombre);
    cont->occupied--;
    DesbloquearSemaforo(cont->id_proceso_3, id);
    if (Front(cont).genero == 'H')
    {
        entraHombre(cont);
    }
    else if (Front(cont).genero == 'M')
    {
        entraMujer(cont);
    }
}
void saleHombre(Controller *cont, int id)
{

    printf("Hombre %s saliendo del servicio higienico\n\n",cont->per[id].nombre);
    cont->occupied--;
    DesbloquearSemaforo(cont->id_proceso_3, id);
    if (Front(cont).genero == 'H')
    {
        entraHombre(cont);
    }
    else if (Front(cont).genero == 'M')
    {
        entraMujer(cont);
    }
}

int main(int argc, char *argv[])
{
    int idShMem, id;
    size_t tabMem;
    key_t llave, key;
    int idSem, idSem2, idSem3;
    char *buf;
    short *vals;
    int miSem;
    int tuSem;
    Controller *controlador;
    key = ftok("shmfile", 65);
    llave = ftok("shmf", 33);

    idShMem = shmget(key, sizeof(Controller), IPC_CREAT | SHM_R | SHM_W);

    controlador = (Controller *)MapearMemoriaComp(idShMem);
    controlador->L = atoi(argv[1]);
    vals=0;
    idSem = CrearSemaforos(1, vals);
    idSem2 = CrearSemaforos(9, vals);
    idSem3 = CrearSemaforos(9, vals);
    controlador->id_proceso_2 = idSem2;
    controlador->id_proceso_3 = idSem3;
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
        
        if (controlador->accion == 'E')
        {
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
                 printf("\n\n");
                
            }
        }
        else if (controlador->accion == 'S')
        {

            if (controlador->per[controlador->id_persona].genero == 'H')
            {

                saleHombre(controlador, controlador->id_persona);
            }
            else if (controlador->per[controlador->id_persona].genero == 'M')
            { 
                saleMujer(controlador, controlador->id_persona);
            }
        }
    }
    BorrarSemaforos(idSem);
    BorrarSemaforos(idSem2);
    BorrarSemaforos(idSem3);
    shmctl(idShMem, IPC_RMID, NULL);
}
