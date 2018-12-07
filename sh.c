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
#include "IO_Personas.c"

/*
 *
 * sh.c es el controlador de la puerta cuya funcion es organizar el uso de 
 * los servicios higienicos sin que haya ninguna situacion bochornosa.
 * este controlador crea 3 grupos de semaforos, el primero controla la puerta
 * y segun sea la accion que desea realizar direcciona a su funcion correspondiente
 * el segundo grupo de parametros desbloqueara a los procesos que buscan entrar al
 * servicio higienico, y el tercer grupo de semaforos, desbloqueara a los procesos 
 * que quieran salir de los servicios higienicos
 * 
 */

int main(int argc, char *argv[])
{
    // Identificadores y claves para la memoria y semaforos compartidos
    int idShMem, id;
    key_t key;
    int idSem, idSem2, idSem3;
    // Sirve para inicializar el grupo de semaforos
    short *vals;

    Controller *controlador;
    key = ftok("shmfile", 65);
    // obtiene un identificador de un segmento nuevo de memoria
    idShMem = ReservarMemoriaComp(key);
    // Mapea o asocia un segmento de memoria compartida al espacio de direcciones
    // de nuestro proceso.
    controlador = (Controller *)MapearMemoriaComp(idShMem);
    // guarda la capacidad del servicio higienico por los parametros que enviamos
    controlador->L = atoi(argv[1]);
    // inicializa los semaforos en 0
    vals = 0;
    // Creacion de los 3 grupos de semaforos
    idSem = CrearSemaforos(1, vals);
    idSem2 = CrearSemaforos(20, vals);
    idSem3 = CrearSemaforos(20, vals);
    // Guardamos los ids de los semaforos
    controlador->id_proceso_2 = idSem2;
    controlador->id_proceso_3 = idSem3;
    *((int *)controlador) = idSem;

    printf("**********************************************************************\n");
    printf("**********************************************************************\n");
    printf("*************CONTROLADOR DE SERVICIOS HIGIENICOS**********************\n");
    printf("**********************************************************************\n");
    printf("**********************************************************************\n\n");
    printf("capacidad para %d personas, ocupadas %d \n", controlador->L, controlador->occupied);
    // Aqui nuestro controlador de la puerta del servicio higienico
    while (1)
    {
        // Guardamos el identificador de nuestra memoria compartida
        *((int *)controlador) = idSem;
        // Este semaforo se desbloquea cuando se dan dos acciones: E entrar
        //  al servicio higienico y S salir del servicio higienico enviados por una persona
        BloquearSemaforo(idSem, 0);
        // Si una persona pide entrar al servicio higienico su accion es E
        if (controlador->accion == 'E')
        {
            // Si hay porlomenos un campo vacio en el servicio higienico
            if (controlador->occupied < controlador->L)
            {
                // Si quiere ingresar un hombre mandamos a entraHombre()
                if (Front(controlador).genero == 'H')
                {
                    entraHombre(controlador);
                }
                // Si quiere ingresar una mujer mandamos a entraMujer()
                else
                {
                    entraMujer(controlador);
                }
            }
            // Si esta lleno que me lo diga y que me muestre las personas que estan esperando los servicios higienicos
            else
            {
                printf("**********************************************************************\n\n");
                printf("Servicios higienicos llenos, espere hasta que se desocupe uno \n\n");
                printf("cola de espera:\n");
                display(controlador);
                printf("**********************************************************************\n");
            }
        }
        // Si una persona quiere salir del servicio higienico su accion es S
        else if (controlador->accion == 'S')
        {
            // Si quiere salir un hombre identificado con el id: controlador->id_persona mandamos a saleHombre()
            if (controlador->per[controlador->id_persona].genero == 'H')
            {

                saleHombre(controlador, controlador->id_persona);
            }
            // Si quiere salir una mujer identificada con el id: controlador->id_persona mandamos a saleMujer();
            else if (controlador->per[controlador->id_persona].genero == 'M')
            {
                saleMujer(controlador, controlador->id_persona);
            }
        }
    }
    // borramos todos los grupos de semaforos y memoria compartida
    BorrarSemaforos(idSem);
    BorrarSemaforos(idSem2);
    BorrarSemaforos(idSem3);
    BorrarMemoriaComp(idShMem);
}
