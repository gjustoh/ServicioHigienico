/*
 *
 * entraMujer() Se encarga de verificar que la que esta a la cabeza de 
 * la cola pueda entrar siempre y cuando haya gente de su mismo sexo o 
 * si no hay nadie en el baño, en este caso pondremos el cartel del baño
 * en M de mujer;luego desbloquea el semaforo que tenia bloqueado en espera de una respuesta
 * y desencola porque ya ingreso al servicio higienico
 * 
 */
void entraMujer(Controller *cont)
{
    // Si no hay nadie pone el cartel en M
    if (cont->occupied == 0)
    {
        cont->gen = 'M';
    }
    // Comprueba que sea la primero o que haya gente de su mismo sexo en
    // el servicio higienico para poder ingresar
    if (cont->occupied == 0 || cont->gen == Front(cont).genero)
    {
        cont->occupied++;
        printf("**********************************************************************\n\n");
        printf("ingreso de %s al servicio higienico \n", Front(cont).nombre);
        printf("baños ocupados  %d de %d\n\n", cont->occupied, cont->L);
        printf("cola de espera:\n");
        display2(cont);
        printf("**********************************************************************\n");
        // Desbloquea el semaforo de eespera a respuesta de ingreso del
        // proceso asociado al id que tiene el de la cabeza dela cola
        DesbloquearSemaforo(cont->id_proceso_2, Front(cont).id);
        // Desencola a la persona que ya entro al servicio higienico
        dequeue(cont);
    }
    // En caso de que sea de otro sexo las personas que estan dentro del servicio higienico
    // envia un mensaje que espear a que salgan los del otro genero.
    else
    {
        printf("**********************************************************************\n\n");
        printf("%s en la cabeza esperando que salgan los hombres:\n\n", Front(cont).nombre);
        printf("**********************************************************************\n");
    }
}
/*
 *
 * entraHombre() es analogo a entraMujer() solo que en este caso 
 * si no hay nadie en el servicio higienico pone su cartel en H.
 * 
 */
void entraHombre(Controller *cont)
{
    // Si no hay nadie pone el cartel en H
    if (cont->occupied == 0)
    {
        cont->gen = 'H';
    }
    // Comprueba que sea la primero o que haya gente de su mismo sexo en
    // el servicio higienico para poder ingresar
    if (cont->occupied == 0 || cont->gen == Front(cont).genero)
    {
        cont->occupied++;
        printf("**********************************************************************\n\n");

        printf("ingreso de %s al servicio higienico \n", Front(cont).nombre);

        printf("baños ocupados  %d de %d\n\n", cont->occupied, cont->L);
        printf("cola de espera:\n");
        display2(cont);
        printf("**********************************************************************\n");
        // Desbloquea el semaforo de eespera a respuesta de ingreso del
        // proceso asociado al id que tiene el de la cabeza dela cola
        DesbloquearSemaforo(cont->id_proceso_2, Front(cont).id);
        // Desencola a la persona que ya entro al servicio higienico
        dequeue(cont);
    }
    // En caso de que sea de otro sexo las personas que estan dentro del servicio higienico
    // envia un mensaje que espear a que salgan los del otro genero.
    else
    {
        printf("**********************************************************************\n\n");
        printf("%s en la cabeza esperando que salgan los hombres:\n\n", Front(cont).nombre);
        printf("**********************************************************************\n");
    }
}
/*
 *
 * saleMujer() Decrementa el contador de ocupados en 1, desbloquea al
 * semaforo que se encarga de esperar respuesta de peticion de salida
 * para que termine su proceso.
 * Luego de salir del servicio higienico comprueba si ahora el que esta
 * en la  cabeza de la cola es hombre o mujer, llama a entraHombre() o
 * entraMujer() respectivamente.
 * 
 */
void saleMujer(Controller *cont, int id)
{
    printf("**********************************************************************\n\n");
    printf("Mujer %s saliendo del servicio higienico\n\n", cont->per[id].nombre);
    printf("**********************************************************************\n");
    cont->occupied--;
    // Desbloquea el semaforo de espera a respuesta de salida del proceso asociado al id
    // que invoco esta llamada
    DesbloquearSemaforo(cont->id_proceso_3, id);
    // llamamos a la funcion entraHombre() o entraMujer()luego de sacar a 
    // la persona del servicio higienico y que desocupa uno de estos
    if (Front(cont).genero == 'H')
    {
        entraHombre(cont);
    }
    else if (Front(cont).genero == 'M')
    {
        entraMujer(cont);
    }
}
/*
 *
 * saleHombre() es analogo a saleMujer().
 * 
 */
void saleHombre(Controller *cont, int id)
{
    printf("**********************************************************************\n\n");
    printf("Hombre %s saliendo del servicio higienico\n\n", cont->per[id].nombre);
    printf("**********************************************************************\n");
    cont->occupied--;
    // Desbloquea el semaforo de espera a respuesta de salida del proceso asociado al id
    // que invoco esta llamada
    DesbloquearSemaforo(cont->id_proceso_3, id);
    // llamamos a la funcion entraHombre() o entraMujer()luego de sacar a 
    // la persona del servicio higienico y que desocupa uno de estos
    if (Front(cont).genero == 'H')
    {
        entraHombre(cont);
    }
    else if (Front(cont).genero == 'M')
    {
        entraMujer(cont);
    }
}