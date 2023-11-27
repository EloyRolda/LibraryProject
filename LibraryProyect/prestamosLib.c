#include "prestamosLib.h"


void prestamoMenu()
{
    arbolNodoLibro * raiz  = inicArbolNodo();
    nodoLibro * lista = inicNodoLibro();

    lista = pasarLibrosArbol(lista);

    int opcion = 0;

    do
    {
        generarTitulo ("PRESTAMOS | DEVOLUCIONES");
        mostrarCadena("1-   PRESTAR LIBRO");
        mostrarCadena("2-   DEVOLUCIONES LIBROS");
        mostrarCadena("3-   VER PRESTAMOS");
       // mostrarCadena("4-   PURGAR DEUDAS SALDADAS");
        mostrarCadena("0-   VOLVER\n");
        asignarValorMsg(&opcion, "Ingrese su opcion");
        switch(opcion)
        {
        case 1:
            lista =  prestarLibro(lista);
            break;
        case 2:
            devolverLibro();
            purgarPrestamos();
            break;
        case 3:
            mostrarArchivoPrestamos();

            break;
        case 4:

            break;

        }
    }
    while(opcion != 0);
}

nodoLibro *prestarLibro(nodoLibro * lista)        //Menu principal de prestar libros
{
    arbolNodoLibro * raiz = inicArbolNodo();
    arbolNodoLibro * libro = inicArbolNodo();
    int ID = 0;
    char categAux[largoCadena];
    int isbn = 0;
    //Pedir categoria
    generarTitulo("PRESTAMOS");
    asignarStringMsg(categAux, "Ingrese la categoria del libro");


    if(categoriaExisteEnLista(categAux, lista))
    {

        raiz = encontrarCategoria(categAux, lista);

        if(raiz != NULL)    //Obtenemos el libro
        {
            do
            {
                generarTitulo("PRESTAMOS");

                asignarValorMsg(&isbn, "Ingrese el isbn del libro");

                libro = obtenerLibroPorISBN(isbn, raiz);

                if(libro == NULL)   //en caso de que no sepa los datos pues se los muestra
                {
                    mostrarCadena("No corresponde a un ISBN valido\n");
                    if(preguntarConfirmacion("Desea buscarlo por categoria | genero para ver su ISBN?"))
                    {
                        verArbolCategoria(categAux, lista);
                        system("pause");
                    }
                }
                else
                {
                    mostrarstLibro(libro->libro);
                }
            }
            while(libro == NULL);       //Dios apiade al bibliotecario que no quiera volver al menu


            //      VERIFICAR DISPONIBILIDAD DE LIBRO

            stPrestamo nuevoPrestamo;
            bool verificado = false;
            if(libro->libro.disponible)
            {

                do
                {

                    //Pide los datos del usuario y los guarda en el archivo
                    generarTitulo("PRESTAMO");
                    asignarValorMsg(&isbn, "Ingrese el ID del usuario"); //isbn ahora es un aux

                    verificado = verificarMiembroID(isbn);

                    if(!verificado)
                    {
                        generarTitulo("PRESTAMO");
                        mostrarCadena("El usuario no existe");
                        colorRojo();
                        system("pause");

                    }
                    else
                    {

                        mostrarMiembroID(isbn);
                        if(preguntarConfirmacion("Es correcto el miembro ingresado?"))
                        {
                            nuevoPrestamo.miembro = obtenerMiembro(isbn);

                            break;
                        }
                        else
                        {
                            verificado = false;
                        }
                    }


                }
                while(!verificado);

                if(nuevoPrestamo.miembro.deudas == 0)
                {
                    nuevoPrestamo.prestado = libro->libro;
                    libro->libro.disponible = false;            //cambio estado del nodo
                    cargarFecha(&nuevoPrestamo.fecha);
                    nuevoPrestamo.retrazo = 0;
                    nuevoPrestamo.reservado.id = 0;     //PARA ASEGURARNOS DE QUE SEA 0 EN LUGAR DE NULL

                    cambiarEstadoLibro(nuevoPrestamo.prestado); //estado del archivo
                    cambiarEstadoMiembro(nuevoPrestamo.miembro.id);
                    nuevoPrestamo.miembro.deudas = 1;

                    agregarPrestamoArchivo(nuevoPrestamo);
                }

            }
            else        //VERIFICA LA DISPONIBILIDAD
            {

                generarTitulo("PRESTAMOS");

                mostrarstLibro(libro->libro);
                mostrarCadena("libro Actualmente prestado");

                if(preguntarConfirmacion("Reservar el libro?"))
                {
                    nuevoPrestamo = obtenerReservaLibro(isbn);      //Obtiene quien reservo el libro

                    if(nuevoPrestamo.reservado.id == 0)
                    {

                        generarTitulo("PRESTAMO");
                        asignarValorMsg(&ID, "Ingrese el ID del usuario");

                        verificado = verificarMiembroID(ID);

                        if(verificado)
                        {
                            generarTitulo("PRESTAMO");
                            mostrarMiembroID(ID);

                            if(preguntarConfirmacion("Es correcto el miembro ingresado?"))
                            {
                                nuevoPrestamo.reservado = obtenerMiembro(ID);
                            }
                            else
                            {
                                verificado = false;
                            }
                        }
                        else
                        {

                            generarTitulo("PRESTAMO");
                            mostrarCadena("El usuario no existe");
                            colorRojo();
                            system("pause");
                            colorBlanco();
                        }




                        if(nuevoPrestamo.reservado.deudas == 0 )
                        {
                            debug("A");
                            agregarReservaArchivo(nuevoPrestamo);
                            debug("B");
                        }
                        else
                        {
                            generarTitulo("PRESTAMO");
                            mostrarCadena("El miembro tiene deudas pendientes");
                            system("pause");

                        }
                    }
                    else
                    {


                        mostrarMiembroID(nuevoPrestamo.reservado.id);
                        mostrarCadena("El libro esta reservado actualmente por:");
                        system("pause");

                    }
                }
            }
        }
    }
    else
    {
        generarTitulo("PRESTAMOS");
        mostrarCadena("La categoria solicitada no existe");
        mostrarCadena("Las Categorias disponibles son:");

        mostrarCategorias(lista);
        system("pause");
    }
    return lista;
}

//-------------------------------------------------------------------------
//                                 stPrestamos
//-------------------------------------------------------------------------


void mostrarCategorias(nodoLibro * lista)
{

    nodoLibro *seguidora = lista;

    if(lista != NULL)
    {

        mostrarCadenaMsg(lista->categoria, "- \n");
        mostrarCategorias(lista->sig);
    }

}


stPrestamo obtenerReservaLibro(int ibsm)
{

    stPrestamo libroPrestado;
    FILE * archivo = fopen(archivoPrestamos, "r+b");
    if(archivo!= NULL){
    while(fread(&libroPrestado, sizeof(stPrestamo),1,archivo) != 0)
    {
        if(ibsm == libroPrestado.prestado.isbn)
        {
            break;
        }
    }
    fclose(archivo);
    }
    return libroPrestado;
}


void imprimirPrestamo(stPrestamo prestamo)
{

    mostrarCadena("| MIEMBRO: ");
    mostrarstMiembro(prestamo.miembro);
    mostrarCadena("| LIBRO: ");
    mostrarstLibro(prestamo.prestado);

    mostrarCadena("| FECHA PRESTAMO");
    printf("|   %i  |   %i  |   %i  |\n", prestamo.fecha.prestamo[0], prestamo.fecha.prestamo[1], prestamo.fecha.prestamo[2]);

    mostrarCadena("| FECHA VENCIMIENTO");
    printf("|   %i  |   %i  |   %i  |\n", prestamo.fecha.vencimiento[0], prestamo.fecha.vencimiento[1], prestamo.fecha.vencimiento[2]);

    if(prestamo.reservado.id ==0)
    {
        mostrarCadena("| NO RESERVADO");

    }
    else
    {
        mostrarCadena("| RESERVADO POR: ");
        mostrarstMiembro(prestamo.reservado);
    }


}



//-------------------------------------------------------------------------
//                                 ARBOLES
//-------------------------------------------------------------------------

arbolNodoLibro * inicArbolNodo()
{
    return NULL;
}

void verArbolCategoria(char categoria[], nodoLibro * lista)
{
    nodoLibro *seguidora = lista;

    if(lista != NULL)
    {
        while(seguidora != NULL)
        {

            if(strcasecmp(categoria, seguidora->categoria) == 0)
            {

                imprimirArbol(seguidora->raiz);
                break;
            }
            seguidora = seguidora->sig;
        }

    }
}

void imprimirArbol(arbolNodoLibro * raiz)
{
    if(raiz!= NULL)
    {
        crearLinea();
        mostrarstLibro(raiz->libro);
        crearLinea();
        imprimirArbol(raiz->menor);
        imprimirArbol(raiz->mayor);
    }

}

arbolNodoLibro * crearNodoArbolLibro(stLibro dato)      //Crea un nodo de un arbol y lo retorna
{
    arbolNodoLibro * nuevoNodoArbol = malloc(sizeof(arbolNodoLibro));
    nuevoNodoArbol->libro = dato;
    nuevoNodoArbol->mayor = inicArbolNodo();
    nuevoNodoArbol->menor = inicArbolNodo();

    return nuevoNodoArbol;
}
arbolNodoLibro * insertarLibroEnArbol(arbolNodoLibro * nuevoLibro, arbolNodoLibro * raiz)
{
    if(raiz != NULL)
    {
        if (nuevoLibro->libro.isbn < raiz->libro.isbn)
        {
            raiz->menor = insertarLibroEnArbol(nuevoLibro, raiz->menor);
        }
        else
        {
            raiz->mayor = insertarLibroEnArbol(nuevoLibro, raiz->mayor);
        }
    }
    else
    {
        raiz = nuevoLibro;
    }
    return raiz;
}

bool nodoArbolLibroExiste(int ibsm, arbolNodoLibro *raiz)
{
    bool existe = false;

    if (raiz != NULL)
    {
        if (ibsm == raiz->libro.isbn)
        {
            existe = true;
        }
        else if (ibsm < raiz->libro.isbn)
        {
            existe = nodoArbolLibroExiste(ibsm, raiz->menor);
        }
        else
        {
            existe = nodoArbolLibroExiste(ibsm, raiz->mayor);
        }
    }
    return existe;
}


arbolNodoLibro * encontrarCategoria(char categoria[], nodoLibro * lista)        //Busca la categoria en la lista y retorna su raiz
{
    arbolNodoLibro *  raiz = inicArbolNodo();

    if(lista != NULL)
    {

        if(strcasecmp(categoria, lista->categoria) == 0)
        {
            printf("a");
            raiz = lista->raiz;
        }
        if(raiz == NULL)
        {
            printf("b");
            raiz =  encontrarCategoria(categoria, lista->sig);
        }
    }

    return raiz;
}

arbolNodoLibro * obtenerLibroPorISBN(int isbn, arbolNodoLibro * raiz)  //rETORNA LA DIRECCION DE MEMORIA DEL LBIRO
{
    arbolNodoLibro * arbolito = inicArbolNodo();

    if (raiz != NULL)
    {
        if (raiz->libro.isbn == isbn)
        {
            arbolito = raiz;
        }
        else
        {
            arbolito = obtenerLibroPorISBN(isbn, raiz->menor);
            if (arbolito == NULL)
            {
                arbolito = obtenerLibroPorISBN(isbn, raiz->mayor);
            }
        }
    }

    return arbolito;
}



//-------------------------------------------------------------------------
//                                 NODOS | LISTA
//-------------------------------------------------------------------------

nodoLibro * pasarLibrosArbol(nodoLibro * lista)
{
    FILE * archivo = fopen(archivoLibros, "rb");
    stLibro libroActual;
    nodoLibro * seguidora = lista;

    if(archivo != NULL)
    {

        while(fread(&libroActual, sizeof(stLibro), 1, archivo)!= 0)
        {
            seguidora = lista;
            if(categoriaExisteEnLista(libroActual.genero, lista))
            {

                while(seguidora != NULL)
                {
                    if(strcasecmp(libroActual.genero, seguidora->categoria) == 0)
                    {

                        seguidora->raiz = cargarLibroArbol(crearNodoArbolLibro(libroActual), seguidora->raiz);
                        break;
                    }
                    seguidora = seguidora->sig;
                }


            }
            else
            {

                lista = crearCategoria(libroActual.genero, lista);
                seguidora = lista;
                while(seguidora != NULL)
                {
                    if(strcasecmp(libroActual.genero, seguidora->categoria)== 0)
                    {

                        seguidora->raiz = cargarLibroArbol(crearNodoArbolLibro(libroActual), seguidora->raiz);

                        break;
                    }
                    seguidora = seguidora->sig;
                }

            }

        }

        fclose(archivo);

    }
    return lista;
}

bool categoriaExisteEnLista(char categoria[], nodoLibro * lista)        //Verifica si la categoria existe en la lista
{
    bool existe = false;
    if(lista != NULL)
    {
        if(strcasecmp(categoria, lista->categoria) == 0)
        {
            existe = true;
        }
        else
        {
            existe = categoriaExisteEnLista(categoria, lista->sig);
        }
    }
    return existe;
}

nodoLibro * crearCategoria(char categoria[], nodoLibro * lista)     //agrega una categoria al final de la lista
{

    if(lista != NULL)
    {
        nodoLibro * seguidora = lista;
        while(seguidora->sig != NULL)
        {
            seguidora = seguidora->sig;
        }
        seguidora->sig = crearNodoLibroParametro(categoria);
    }
    else
    {
        lista = crearNodoLibroParametro(categoria);
    }
    return lista;
}

nodoLibro * crearNodoLibroParametro(char categoria[])       //crea un nodoLibro con la categoria cargada
{

    nodoLibro * nuevoNodo = malloc(sizeof(nodoLibro));

    strcpy(nuevoNodo->categoria, categoria);

    nuevoNodo->sig = inicNodoLibro();
    nuevoNodo->raiz = inicArbolNodo();

    return nuevoNodo;

}

nodoLibro * inicNodoLibro()
{
    return NULL;
}

arbolNodoLibro * cargarLibroArbol(arbolNodoLibro * nuevoLibro, arbolNodoLibro * raiz)
{

    if(!nodoArbolLibroExiste(nuevoLibro->libro.isbn, raiz))
    {

        raiz = insertarLibroEnArbol(nuevoLibro, raiz);

    }
    return raiz;
}


//-------------------------------------------------------------------------
//                                  ARCHIVOS
//-------------------------------------------------------------------------

void agregarPrestamoArchivo(stPrestamo prestamo)
{

    FILE * archivo = fopen(archivoPrestamos, "ab");

    if(archivo != NULL)
    {

        fseek(archivo, 0, SEEK_END);
        fwrite(&prestamo, sizeof(stPrestamo), 1, archivo);
        fclose(archivo);
    }
}


void agregarReservaArchivo(stPrestamo prestamo)
{

    FILE * archivo = fopen(archivoPrestamos, "r+b");
    stPrestamo aux;
    if(archivo != NULL)
    {
        while(fread (&aux,sizeof(stPrestamo), 1, archivo) != 0)
        {
            if(aux.miembro.id == prestamo.miembro.id)
            {
                fseek(archivo, sizeof(stPrestamo)*-1, SEEK_CUR);
                fwrite(&prestamo, sizeof(stPrestamo), 1, archivo);
                break;
            }

        }

        fclose(archivo);
    }
}

bool prestamoExiste(int id)
{
    bool existe = false;
    FILE * archivo = fopen(archivoPrestamos, "ab");
    stPrestamo prestamo;
    if(archivo != NULL)
    {
        while(fread(&prestamo, sizeof(stPrestamo),1,archivo) != 0)
        {
            if(id == prestamo.miembro.id)
            {
                existe = true;
            }
        }
        fclose(archivo);
    }
    return existe;
}

void mostrarArchivoPrestamos()
{

    FILE * archivo = fopen(archivoPrestamos, "rb");
    stPrestamo prestamo;
    generarTitulo("VER PRESTAMOS\n");
    if(archivo != NULL)
    {
        while(fread(&prestamo, sizeof(stPrestamo),1,archivo) != 0)
        {
            if(prestamo.miembro.id != 0){
            crearLinea();
            imprimirPrestamo(prestamo);
            crearLinea();
            }
        }
        fclose(archivo);
        system("pause");
    }
    else
    {

        generarTitulo("PRESTAMOS");
        mostrarCadena("Error al intentar abrir el archivo prestamos");
        system("pause");
    }
}

void purgarPrestamos()
{

    FILE * archivo = fopen(archivoPrestamos, "r+b");
    char archivoAux[largoCadena];
    stPrestamo prestado;
    if (archivo != NULL)
    {

        generarTitulo("PURGA DE PRESTAMOS");
        mostrarCadena("Purga exitosa");
        colorVerde();
        system("pause");

        strcpy(archivoAux, "aux.bin");

        while (fread(&prestado, sizeof(stPrestamo), 1, archivo))
        {

            if (prestado.miembro.id != 0)
            {
                FILE * archivoAux = fopen(archivoAux, "w+b");
                fwrite(&prestado, sizeof(stPrestamo), 1, archivoAux);
                fclose(archivoAux);
            }

        }

        fclose(archivo);

        remove(archivoPrestamos);
        rename(archivoAux, archivoPrestamos);

    }
    else
    {

        generarTitulo("PURGA DE PRESTAMOS");
        mostrarCadena("Error al intentar abrir los archivos");
        system("pause");

    }
    colorBlanco();
}


//-------------------------------------------------------------------------
//                                  MIEMBROS
//-------------------------------------------------------------------------

void cambiarEstadoMiembro(int id)
{

    FILE * archivo = fopen(archivoMiembros, "r+b");
    stMiembro miembro;
    if(archivo != NULL)
    {
        while(fread(&miembro, sizeof(stMiembro),1, archivo )!= 0)
        {
            if(id == miembro.id)
            {
                miembro.deudas = 1;
                fseek(archivo, sizeof(stMiembro) * -1, SEEK_CUR);
                fwrite(&miembro, sizeof(stMiembro),1,archivo);
                break;
            }
        }
        fclose(archivo);
    }
}


//-------------------------------------------------------------------------
//                                  LIBROS
//-------------------------------------------------------------------------

void cambiarEstadoLibro(stLibro libro)
{
    FILE *archivo = fopen(archivoLibros, "r+b");
    stLibro aux;
    if(archivo != NULL)
    {
        while(fread(&aux, sizeof(stLibro), 1, archivo) != 0)
        {
            if(aux.isbn == libro.isbn)
            {
                aux.disponible = !aux.disponible;
                fseek(archivo, sizeof(stLibro)* -1, SEEK_CUR);
                fwrite(&aux, sizeof(stLibro),1,archivo);
                break;
            }
        }
        fclose(archivo);
    }
}

//-------------------------------------------------------------------------
//                                  FECHAS
//-------------------------------------------------------------------------

void cargarFecha(stFecha *fecha)
{
    // Obtener la hora actual
    time_t tiempoActual = time(NULL);

    // Obtener el día, el mes y el año
    int dia = localtime(&tiempoActual)->tm_mday;
    int mes = 0; // Ajuste para mes de 1 a 12
    int anio = localtime(&tiempoActual)->tm_year + 1900;

    mes = localtime(&tiempoActual)->tm_mon +1;
    if(mes != 0)
    {
        mes ++;
    }
    else
    {
        mes = 1;
    }

    int diaVencimiento = dia + tiempoPrestamo;

    // Validar el mes

    if(mes == 1 || mes == 3  || mes == 5  || mes ==  7 || mes ==  8 || mes == 10|| mes ==  12)
    {
        if(diaVencimiento> 31)
        {
            diaVencimiento = diaVencimiento - 31;
            fecha->vencimiento[1] = mes+1;
        }
    }
    else if(mes == 4 || mes == 6 || mes == 11)
    {
        if((diaVencimiento)> 30)
        {
            diaVencimiento = diaVencimiento - 30;
            fecha->vencimiento[1] = mes+1;
        }
    }
    else
    {
        if((diaVencimiento)> 29)
        {
            diaVencimiento = diaVencimiento - 29;
            fecha->vencimiento[1] = mes+1;
        }
    }
    if(fecha->vencimiento[1] > 12)
    {
        fecha->vencimiento [1] = 1;
        fecha->vencimiento[2] = anio +1;
    }
    else
    {
        fecha->vencimiento[2] = anio;
    }

    // Asignar los valores a la estructura
    fecha->prestamo[0] = dia;
    fecha->prestamo[1] = mes;
    fecha->prestamo[2] = anio;
    fecha->vencimiento[0] = diaVencimiento;


}

void extenderPlazo(stFecha *fecha)
{
    int dia = fecha ->vencimiento[0];
    int mes = fecha ->vencimiento[1];
    int anio = fecha ->vencimiento[2];

    int diaVencimiento = dia + tiempoPrestamo;

    if(mes == 1 || mes == 3  || mes == 5  || mes ==  7 || mes ==  8 || mes == 10|| mes ==  12)
    {
        if(diaVencimiento> 31)
        {
            diaVencimiento = diaVencimiento - 31;
            fecha->vencimiento[1] = mes+1;
        }
    }
    else if(mes == 4 || mes == 6 || mes == 11)
    {
        if((diaVencimiento)> 30)
        {
            diaVencimiento = diaVencimiento - 30;
            fecha->vencimiento[1] = mes+1;
        }
    }
    else
    {
        if((diaVencimiento)> 29)
        {
            diaVencimiento = diaVencimiento - 29;
            fecha->vencimiento[1] = mes+1;
        }
    }
    if(fecha->vencimiento[1] > 12)
    {
        fecha->vencimiento [1] = 1;
        fecha->vencimiento[2] = anio +1;
    }
    else
    {
        fecha->vencimiento[2] = anio;
    }

    // Asignar los valores a la estructura

    fecha->vencimiento[0] = diaVencimiento;
}


//---------------------------------------------------------------------------------
//                                  DEVOLUCIONES
//---------------------------------------------------------------------------------


void devolverLibro()
{

    FILE * archivo = fopen(archivoPrestamos, "r+b");
    int id = 0;
    stPrestamo prestao;
    if(archivo != NULL)
    {
        generarTitulo("DEVOLUCIONES");

        asignarValorMsg(&id,"Ingrese el ID del miembro");

        if(verificarMiembroID(id))
        {
            while(fread(&prestao, sizeof(stPrestamo), 1,archivo)!= 0)
            {

                if(id == prestao.miembro.id)
                {
                    fseek(archivo, sizeof(stPrestamo)*-1,SEEK_CUR);
                    imprimirPrestamo(prestao);

                    if(preguntarConfirmacion("Eliminar deudas?"))
                    {
                        eliminarDeudaMiembro(id);
                        eliminarDeudorArchivo(id);
                    }
                    break;
                }
            }

        }
        else
        {
            generarTitulo("DEVOLUCIONES");
            mostrarCadena("Error al intentar abrir el archivo devoluciones");
            system("pause");

        }


        fclose(archivo);
    }
    else
    {
        generarTitulo("DEVOLUCIONES");
        mostrarCadena("No hay libros pendientes para devolver");
        system("pause");

    }
}



void eliminarDeudaMiembro(int ID)
{

    FILE * archivo = fopen(archivoMiembros, "r+b");

    stMiembro sucioDeudor;

    if(archivo != NULL)
    {

        while(fread(&sucioDeudor, sizeof(stMiembro), 1, archivo))
        {
            if(sucioDeudor.id == ID)
            {

                fseek(archivo, sizeof(stMiembro) * -1, SEEK_CUR);
                sucioDeudor.deudas = 0; //Claramente se asume que se le dio dinero en mano
                fwrite(&sucioDeudor, sizeof(stMiembro), 1, archivo);
                break;
            }
        }
        fclose(archivo);
    }
}

void eliminarDeudorArchivo(int ID)
{

    FILE * archivo = fopen(archivoPrestamos, "r+b");

    stPrestamo sucioDeudor;

    if(archivo != NULL)
    {

        while(fread(&sucioDeudor, sizeof(stPrestamo), 1, archivo))
        {
            if(sucioDeudor.miembro.id == ID)
            {
                fseek(archivo, sizeof(stPrestamo) * -1, SEEK_CUR);
                generarTitulo("AVISO DE RESERVA");
                if(sucioDeudor.reservado.id != 0)
                {
                    mostrarCadena("AVISAR DE DISPONIBILIDAD DE LIBRO A:");
                    mostrarstMiembro(sucioDeudor.reservado);
                    colorAmarillo();
                    system("pause");
                    colorBlanco();
                }
                cambiarEstadoLibro(sucioDeudor.prestado);
                sucioDeudor.miembro.id = 0; //Claramente se asume que se le dio dinero en mano
                fwrite(&sucioDeudor, sizeof(stPrestamo), 1, archivo);
                break;
            }
        }
        fclose(archivo);
    }
}




