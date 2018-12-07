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
union semun {
    int val;               //valor para SETVAL
    struct semid_ds *buf;  //buffer para IPC_STAT, IPC_SET
    unsigned short *array; //arreglo para GETALL, SETALL
    struct seminfo *__buf; //buffer para IPC_INFO
};
//declaraciones de las funciones wrappers para simplificarnos la vida
int ReservarMemoriaComp(int n)
{
    return shmget(IPC_PRIVATE, n, IPC_CREAT | SHM_R | SHM_W);
}
void *MapearMemoriaComp(int id)
{
    void *addr;
    addr = shmat(id, NULL, 0);  //adjuntamos el segmento
    //  shmctl(id, IPC_RMID, NULL); //y lo marcamos para que se destruya
    return addr;
}
int ReservarMemoriaCompT(size_t size)
{
    return shmget(IPC_PRIVATE, size, IPC_CREAT | 0600);
}
int CrearSemaforo1(key_t key,int n){
    semget(key, n, SHM_R | SHM_W);
}
int CrearSemaforos(int n, short *vals)
{
    union semun arg;
    int id;
    id = semget(IPC_PRIVATE, n, SHM_R | SHM_W);
    arg.array = vals;
    semctl(id, 0, SETALL, arg);
    return id;
}
int CrearSemaforos2(int n, short *vals)
{
    union semun arg;
    int id;
    id = semget(IPC_PRIVATE, n, SHM_R | SHM_W);
    arg.array = vals;
    semctl(id, 0, SETALL, arg);
    return id;
}
void BorrarSemaforos(int id)
{
    if (semctl(id, 0, IPC_RMID, NULL) == -1)
    {
        perror("Error liberando semáforo!");
        exit(EXIT_FAILURE);
    }
}
void BloquearSemaforo(int id, int i)
{
    struct sembuf sb;
    sb.sem_num = i;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;
    semop(id, &sb, 1);
}
void DesbloquearSemaforo(int id, int i)
{
    struct sembuf sb;
    sb.sem_num = i;
    sb.sem_op = 1;
    sb.sem_flg = SEM_UNDO;
    semop(id, &sb, 1);
}

enum
{
    SEM_USER_1, //el turno de la primera persona
    SEM_USER_2  //el turno de la 2da persona
};
