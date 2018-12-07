#define MAXSIZE 50
/*
 *
 *Estructura Persona datos:
 *nombre: nombre asignado
 *genero: H si es hombre y M si es mujer
 *id: identificador de la persona para el uso de semaforos
 *isOcuped: marca si esta ocupado o no la persona
 * 
 */
typedef struct
{
    char nombre[256];
    char genero;
    int id;
    char isOcuped;
} Persona;
/*
 *
 * id_proceso: guarda el valor de la memoria compartida, y el valor del primer grupo de semaforos
 *id_proceso_2: guarda el valor del identificador del segundo grupo de semaforos que controla el mensaje que recibe la persona que entro al baño
 *id_proceso_3: guarda el valor del identificador del tercer grupo de semaforosque controla el mensaje que recibe la persona que salio al baño
 *gen: genero que se encuentra usando los servicios higienicos 
 *L: valor de la capacidad del baño
 *occupied: cantidad de banios que se estan usando 
 *per[MAXSIZE]: cola de personal que esperan ingresar a los servicios higienicos, MAXSIZE tamaño maximo que pueda tener la cola 
 *front: identificador de la persina que se encuentra al inicio de la cola 
 *rear: analogo a front, pero apunta al final
 *size: cantidad de personas en la cola 
 *cont: identificador que incrementara por cada persona ingresada, y se asignara al campo id de Persona
 *accion: valor para identificar que accion esta aciendo, puede ser: 'E' entrar al baño o 'S' salir 
 *id_persona: servira para identificar a que persona estaremos sacando del baño
 */
typedef struct
{
    int id_proceso;
    int id_proceso_2;
    int id_proceso_3;
    char gen;
    int L;
     int occupied;
    Persona per[MAXSIZE];
    int front;
    int rear;
    int size;
    int cont;
    char accion;
    int id_persona;
} Controller;

/*
 *
 * pregunta si esta vacio o no nuestra cola
 * 
 */
int isempty(Controller *cont)
{
    return cont->size <= 0;
}
/* 
 * 
 * pregunta si esta lleno nuestra cola
 * 
 */
int isfull(Controller *cont)
{
    return cont->size == MAXSIZE;
}
/* 
 * 
 * encola una nueva persona donde:
 * cont: puntero a la memoria compartida
 * _per: Persona a ser encolada
 * 
 */

void enqueue(Controller *cont, Persona _per)
{
    // si la cola aun no esta llena
    if (cont->size < MAXSIZE)
    {
        // si esta vacia la cola aumentamos el tamaño
        //inicializamos las posiciones final e inicial en 0
        //ponemos en la posicion 0 a la nueva persona
        if (cont->size <= 0&&cont->front==0)
        {
            cont->per[0] = _per;
            cont->front = cont->rear = 0;
            cont->size = 1;
        }
        //Cuando la parte trasera es la ultima pero aun la cola no esta llena
        //La nueva persona se agrega en la primera posicion  asignamos rear a 0
        //aumentamos el tamaño, pero no tocamos al front
        else if (cont->rear == MAXSIZE - 1)
        {
            cont->per[0] = _per;
            cont->rear = 0;
            cont->size++;
        }
        //En caso general se agrega en al siguiente posicion del ultimo elemento agregado
        //se aumenta el tamaño y la posicion trasera
        else
        {
            cont->per[cont->rear+1] = _per;
            cont->rear++;
            cont->size++;
        }
    }
    //mensaje si la cola esta llena
    else
    {
        printf("La cola está llena \n ");
    }
}
/*
 * 
 * Desencolamos a la primera persona que ingreso a nuestra cola
 * 
 */
Persona dequeue(Controller *cont)
{
    //Si la cola esta vacia muestra un mensaje
    if (cont->size < 0)
    {
        printf("Queue is empty\n");
    }
    //Si hay elementos en la cola
    //disminuye la cantidad de personas en nuestra cola
    //aumenta la posicion del que esta en la cabeza
    //ponemos en ocupado
    else
    {
        cont->size--;
        cont->front++;
        cont->per[cont->front].isOcuped='S';
    }
}

/*
 * 
 * Devuelve a la persona que se encuentra en la primera posicion de nuestra cola
 * 
 */

Persona Front(Controller *cont)
{
    
    return cont->per[cont->front];
}

/*
 * 
 * Metodos display para mostrar la cola
 * 
 */

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
    
     }
}
