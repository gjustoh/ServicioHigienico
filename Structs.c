#define MAXSIZE 20
typedef struct
{
    char nombre[256];
    char genero;
    int id;
    char isOcuped;
} Persona;

typedef struct
{
    int id_proceso;
    char gen;
    int L;
     int occupied;
    Persona per[MAXSIZE];
    int front;
    int rear;
    int size;
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
            printf("%s %d\n", cont->per[i].nombre,i);
        }
    }
    else
    {
        for (i = cont->front; i < MAXSIZE; i++)
        {
            printf("%s %d\n", cont->per[i].nombre,i);
        }
        for (i = 0; i <= cont->rear; i++)
        {
            printf("%s %d\n", cont->per[i].nombre,i);
        }
    }
}
