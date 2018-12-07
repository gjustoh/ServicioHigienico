#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
/*
 *
 * definimos la union que consta de diferentes tipos con la finalidad de crear varios semaforos e inicializarlos:
 *  
 */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

/*
 *
 * Reservar memoria compartida
 * n: tamaño para reservar en la memoria compartida
 *  
 */

int ReservarMemoriaComp(int n)
{
    return shmget(IPC_PRIVATE, n, IPC_CREAT | SHM_R | SHM_W);
}

/*
 *
 * Mapear memoria compartida donde:
 * id: identificador para compartir la memoria entre varios procesos
 * 
 */

void *MapearMemoriaComp(int id)
{
    void *addr;
    addr = shmat(id, NULL, 0);
    return addr;
}

/*
 *
 * Crear los grupos de semaforos donde:
 * n: cantidad desemaforos para el grupo
 * vals: inicializacion de cada semaforo
 * 
 */

int CrearSemaforos(int n, short *vals)
{
    union semun arg;
    int id;
    id = semget(IPC_PRIVATE, n, SHM_R | SHM_W);
    arg.array = vals;
    semctl(id, 0, SETALL, arg);
    return id;
}
/*
 *
 * borrar el grupos de semaforos mediante un identificador donde:
 * id: identificador del grupo de semaforos para ser borrados
 * 
 */
void BorrarSemaforos(int id)
{
    if (semctl(id, 0, IPC_RMID, NULL) == -1)
    {
        perror("Error liberando semáforo!");
        exit(EXIT_FAILURE);
    }
}
/*
 *
 * borrar memoria compartida mediante un identificador donde:
 * id: identificador de la memoria compartida
 * 
 */
void BorrarMemoriaComp(int id)
{
    if (shmctl(id, IPC_RMID, NULL) == -1)
    {
        perror("Error liberando memoria!");
        exit(EXIT_FAILURE);
    }
}

/*
 *
 * bloquear un semaforo especifico donde:
 * id: identificador del grupo de semaforos
 * i: poscicion del semaforo en el arreglo para ser bloqueado
 *  
 */

void BloquearSemaforo(int id, int i)
{
    struct sembuf sb;
    sb.sem_num = i;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;
    semop(id, &sb, 1);
}
/*
 *
 * desbloquear un semaforo especifico donde:
 * id: identificador del grupo de semaforos
 * i: poscicion del semaforo en el arreglo para ser desbloqueado
 *  
 */
void DesbloquearSemaforo(int id, int i)
{
    struct sembuf sb;
    sb.sem_num = i;
    sb.sem_op = 1;
    sb.sem_flg = SEM_UNDO;
    semop(id, &sb, 1);
}
