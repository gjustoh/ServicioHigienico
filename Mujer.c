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
/* 
 * 
 * Mujer.c es analogo aHombre.c envez de crear un Hombre crea una mujer 
 * 
 */
int main(int argc, char *argv[])
{
    //creacion de los id para la memoria compartida, identificador de la persona, id del semaforo
    int idShMem, id, idSem;
    Controller *controlador;
    key_t key = ftok("shmfile", 65);
    // obtiene un identificado de un segmento que ya existe
    idShMem = ReservarMemoriaComp(key);
    // Mapea o asocia un segmento de memoria compartida al espacio de direcciones
    // de nuestro proceso.
    controlador = (Controller *)MapearMemoriaComp(idShMem);
    //guarda el identificador de la memoria compartida
    idSem = *((int *)controlador);
    //crea una nueva pesona y se le asigna su id
    Persona mujer;
    mujer.id = controlador->cont;
    //guardamos en id su identificador
    id = controlador->cont;
    //guardamos su nombre que ingresamo por teclado, le ponemos de genero hombre y encolamosa nuestra cola
    strcpy(mujer.nombre, argv[1]);
    mujer.genero = 'M';
    enqueue(controlador, mujer);
    controlador->cont++;
    printf("Bienvenida  %s  entrando a los SERVICIOS HIGIENICOS\n\n", mujer.nombre);
    printf("Esperando entrar al servicio higienico\n\n");
    //Enviamos el tipo de accion que queremos realizar en este caso es la E de entrar al baño
    controlador->accion = 'E';
    //Desbloqueamos el controlador de la puerta
    DesbloquearSemaforo(idSem, 0);
    // Nos bloqueamos, espearmos a que nos den la confirmacion de que podemos entrar al baño
    BloquearSemaforo(controlador->id_proceso_2, id);
    printf("usted entro al servicios higienico\n\n");
    // Preguntamos si quiere salir del baño
    while (1)
    {
        printf("Desea salir del baño?\n");
        printf(">");
        char *tmp;
        fgets(tmp, BUFSIZ, stdin);
        // si su respuesta es si : s entonces guardamos que la accion es S de salir, y
        // enviamos el id de la persona del proceso,una vez guardado esta informacion
        // desbloqueamos el controlador de la puerta
        if (tmp[0] == 's')
        {
            controlador->accion = 'S';
            controlador->id_persona = id;
            DesbloquearSemaforo(idSem, 0);
            break;
        }
    }
    // Nos bloqueamos para esperar la confirmacion de que yapodemos irnosde losservicios higienicos
    BloquearSemaforo(controlador->id_proceso_3, id);
    printf("\n\nUsted salio de los servicios higienicos\n\n");
}
