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
    int idShMem, id;
    int idSem;
    Controller *controlador;
    key_t key = ftok("shmfile", 65);

    idShMem = shmget(key, sizeof(Controller), IPC_CREAT | SHM_R | SHM_W);
    controlador = (Controller *)MapearMemoriaComp(idShMem);
    idSem = *((int *)controlador);

    Persona hombre;
    hombre.id = controlador->cont;

    id = controlador->cont;
    strcpy(hombre.nombre, argv[1]);
    hombre.genero = 'H';
    enqueue(controlador, hombre);
    controlador->cont++;
    printf("Bienvenido  %s  entrando a los SERVICIOS HIGIENICOS\n\n", hombre.nombre);
    printf("Esperando entrar al servicio higienico\n\n");
    controlador->accion = 'E';
    DesbloquearSemaforo(idSem, 0);
    BloquearSemaforo(controlador->id_proceso_2, id);
    printf("usted entro al servicios higienico %d \n\n", id);
    while (1)
    {
        printf("Desea salir del baño?\n");
        printf(">");
        char *tmp;
        fgets(tmp, BUFSIZ, stdin);
        if (tmp[0] == 's')
        {
            controlador->accion = 'S';
            controlador->id_persona = id;
            DesbloquearSemaforo(idSem, 0);
            break;
        }
    }
    BloquearSemaforo(controlador->id_proceso_3, id);
    printf("\n\nUsted salio de los servicios higienicos\n\n");
}
