#define MAXSIZE 20
/*
 *Estructura Persona datos:
 *nombre: nombre asignado
 *genero: H si es hombre y M si es mujer
 *id: identificador de la persona para el uso de semaforos
 *isOcuped:"posible valor que no sirva"->esto nolo uso todavia, creo que lo eliminare 
 * 
 */
typedef struct
{
    char nombre[256];
    char genero;
    int id;
    char isOcuped;
} Persona;
/*id_proceso: guarda el valor de la memoria compartida, y el valor del primer grupo de semaforos
 *id_proceso_2: guarda el valor del identificador del segundo grupo de semaforos
 *gen: genero que se encuentra usando los servicios higienicos 
 *L: valor de la capacidad del baño
 *occupied: cantidad de banios que se estan usando 
 *per[MAXSIZE]: cola de personal que esperan ingresar a los servicios higienicos, MAXSIZE tamaño maximo que pueda tener la cola 
 *front: identificador de la persina que se encuentra al inicio de la cola 
 *rear: analogo a front, pero apunta al final
 *size: cantidad de personas en la cola 
 *cont: identificador que incrementara por cada persona ingresada, y se asignara al campo id de Persona 
 */
typedef struct
{
    int id_proceso;
    int id_proceso_2;
    char gen;
    int L;
     int occupied;
    Persona per[MAXSIZE];
    int front;
    int rear;
    int size;
    int cont;
} Controller;

int isempty(Controller *cont)
{
    return cont->size <= 0;
}
int isfull(Controller *cont)
{
    return cont->size == MAXSIZE;
}
void enqueue(Controller *cont, Persona _per)
{
    if (cont->size < MAXSIZE)
    {
        if (cont->size <= 0)
        {
            cont->per[0] = _per;
            cont->front = cont->rear = 0;
            cont->size = 1;
        }
        else if (cont->rear == MAXSIZE - 1)
        {
            cont->per[0] = _per;
            cont->rear = 0;
            cont->size++;
        }
        else
        {
            cont->per[cont->rear + 1] = _per;
            cont->rear++;
            cont->size++;
        }
    }
    else
    {
        printf("La cola está llena \n ");
    }
}
Persona dequeue(Controller *cont)
{
    if (cont->size < 0)
    {
        printf("Queue is empty\n");
    }
    else
    {
        cont->size--;
        cont->front++;
        cont->per[cont->front].isOcuped='S';
    }
}
Persona Front(Controller *cont)
{
    // printf("%d\n", cont->front);
    
    return cont->per[cont->front];
}
void display(Controller *cont)
{
    int i;
    if (cont->rear >= cont->front)
    {
        for (i = cont->front; i <= cont->rear; i++)
        {
            printf("%s\n", cont->per[i].nombre);
        }
    }
     else
     {
        printf("NO HAY PERSONAS ESPERANDO\n\n");
    //     for (i = cont->front; i < MAXSIZE; i++)
    //     {
    //         printf("->%s %d\n", cont->per[i].nombre,i);
    //     }
    //     for (i = 0; i <= cont->rear; i++)
    //     {
    //         printf("%s %d\n", cont->per[i].nombre,i);
    //     }
     }
}
void display2(Controller *cont)
{
    int i;
    if (cont->rear >= cont->front)
    {
        for (i = cont->front+1; i <= cont->rear; i++)
        {
            printf("%s\n", cont->per[i].nombre);
        }
    }
     else
     {
        printf("NO HAY PERSONAS ESPERANDO\n\n");
    //     for (i = cont->front; i < MAXSIZE; i++)
    //     {
    //         printf("->%s %d\n", cont->per[i].nombre,i);
    //     }
    //     for (i = 0; i <= cont->rear; i++)
    //     {
    //         printf("%s %d\n", cont->per[i].nombre,i);
    //     }
     }
}
