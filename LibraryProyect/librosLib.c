#include "librosLib.h"


void menuLibros()
{

    int opcion = 0;


    do
    {
        generarTitulo("Menu Gestion Libros");

        mostrarCadena("1- Registrar Libros");
        mostrarCadena("2- Ver registro de Libros");
        mostrarCadena("3- Ver Libro deseado");
        mostrarCadena("4- Modificar Libro");
        mostrarCadena("5- Eliminar Libro");
        mostrarCadena("0- Volver");
        printf("\n");

        asignarValorMsg(&opcion, "Ingrese su opcion");

        switch(opcion)
        {
        case 1:
            registrarLibro();
            break;
        case 2:
            mostrarRegistroLibros();
            system("pause");
            break;
        case 3:
            menuMostrarPorCampoLibro();
            break;
        case 4:
            modificarLibroEnRegistro();
            break;
               case 5:

                eliminarLibroDeArchivo();

                break;
        default:
            if(opcion != 0)
            {
                informarValorInvalido();
            }
            break;
        }
    }
    while(!rangoValido(0,5,opcion) || opcion != 0);
}

void registrarLibro()       //Permite registrar un libro en el archivo
{

    stLibro nuevoLibro;
    cargarstLibro(&nuevoLibro);

    if(isbnExisteEnArchivo(nuevoLibro.isbn))
    {
        generarTitulo("Libro Ya Existente");
        colorRojo();
        system("pause");
        colorBlanco();
    }
    else
    {

        mostrarCadena("No existe");
        generarTitulo("Libro Agregado");
        agregarLibroArchivo(nuevoLibro);
        colorVerde();
        system("pause");
        colorBlanco();
    }




}
//      AGREGAR A ARCHIVO

void agregarLibroArchivo(stLibro libroAgregable)        //Agrega al archivo el libro YA VERIFICADO DE ANTEMANO
{
    FILE * archivo = fopen(archivoLibros, "ab");
    if(archivo != NULL)
    {
        fwrite(&libroAgregable, sizeof(stLibro), 1, archivo);

        fclose(archivo);
    }
}



//CARGA DE CAMPOS

void cargarstLibro(stLibro * nuevoLibro)    //carga todos los campos de stLibro
{

    int cont = 0;
    generarTitulo("Nuevo Libro");

    asignarStringMsg(nuevoLibro->titulo,    "Ingrese el titulo del Libro");

    asignarStringMsg(nuevoLibro->autor,     "Ingrese el nombre del autor");

    asignarStringMsg(nuevoLibro->genero,    "Ingrese el genero del Libro");

    asignarValorMsg(&nuevoLibro->isbn, "Ingrese el codigo ISBN");

    nuevoLibro->disponible = true;
}


//      VALIDACIONES

bool isbnExisteEnArchivo(int isbnABuscar)          //Revisa en el archivo si existe un bdsm igual
{
    FILE * archivo = fopen(archivoLibros, "rb");
    stLibro auxLibro;

    bool existe = false;
    if(archivo != NULL)
    {
        while(fread(&auxLibro, sizeof(stLibro), 1, archivo) != 0 && !existe)
        {
            if(isbnABuscar == auxLibro.isbn)
            {

                existe = true;
                break;
                debug("LO ENCONTRE PAAAA");
            }
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoLibros();
    }

    return existe;
}


bool stringExisteEnArchivoLibro(char cadena[], char tipo[])     //verifica si el string existe mediante el tipo, titulo, autor u genero
{
    bool existe = false;
    stLibro auxLibro;
    FILE * archivo = fopen(archivoLibros, "rb");
    if(archivo != NULL)
    {
        while(fread(&auxLibro, sizeof(stLibro),1,archivo)!=0)
        {
            if(strcasecmp(tipo,     "Titulo") == 0)
            {
                if( strcasecmp(auxLibro.titulo, cadena)==0)
                {
                    existe = true;
                }
            }
            //Por autor
            if(strcasecmp(tipo,   "Autor") == 0)
            {
                if( strcasecmp(auxLibro.autor, cadena)==0)
                {
                    existe = true;

                }
            }
            if(strcasecmp(tipo,   "Genero") == 0)
            {
                if( strcasecmp(auxLibro.genero, cadena)==0)
                {
                    existe = true;
                }
            }
        }
    }


    return existe;
}

//      MOSTRAR POR PANTALLA

void mostrarRegistroLibros()        //Muestra el archivo libro por pantalla con todos sus elementos que esten cargados
{

    FILE * archivo = fopen(archivoLibros, "rb");

    stLibro libroActual;
    generarTitulo("Libros Cargados En Archivo");
    if(archivo != NULL)
    {
        while(fread(&libroActual, sizeof(stLibro), 1, archivo))
        {
            mostrarstLibro(libroActual);
        }
    }
    else
    {
        informarErrorAperturaArchivoLibros();

    }

}

void mostrarstLibro(stLibro libro)      //Muestra la estructura libro
{
    mostrarCadenaMsg("|    Titulo:     ", libro.titulo);
    mostrarCadenaMsg("|    Autor:      ", libro.autor);
    mostrarCadenaMsg("|    Genero:     ", libro.genero);
    printf("|    ISBN:       %i\n", libro.isbn);

    if(libro.disponible)
    {

        mostrarCadena("|    Estado:     Disponible");
    }
    else
    {

        mostrarCadena("|    Estado:     NO DISPONIBLE");
    }
    printf("\n\n");


}
//      MOSTRAR POR PARAMETRO

void menuMostrarPorCampoLibro() //Muestra segun el campo el / los libros pertinentes
{

    int opcion = 0;
    char auxString[largoCadena];
    int auxValor;

    do
    {
        generarTitulo("Mostrar Por Campo");
        fflush(stdin);

        //Opciones
        mostrarCadena("1- Mostrar por TITULO");
        mostrarCadena("2- Mostrar por AUTOR");
        mostrarCadena("3- Mostrar por GENERO");
        mostrarCadena("4- Mostrar por ISBN");
        mostrarCadena("5- Mostrar DISPONIBLES");
        mostrarCadena("0- Volver");
        //FinOpciones

        printf("\n");
        asignarValorMsg(&opcion, "Ingrese su opcion");
        switch(opcion)
        {
        case 1:
            generarTitulo("Busqueda por TITULO ");
            asignarStringMsg(auxString,         "Ingrese TITULO a buscar en el registro\nEl Titulo debe coincidir con el registro");
            if(stringExisteEnArchivoLibro(auxString, "Titulo"))
            {
                mostrarLibroPorTitulo(auxString);
            }
            else
            {
                generarTitulo("Busqueda por TITULO ");
                mostrarCadena("\nNo se a encontrado ese TITULO\n");
            }

            break;
        case 2:
            generarTitulo("Busqueda por AUTOR ");
            asignarStringMsg(auxString,         "Ingrese AUTOR a buscar en el registro");
            if(stringExisteEnArchivoLibro(auxString, "Autor"))
            {
                mostrarLibroPorAutor(auxString);
            }
            else
            {
                generarTitulo("Busqueda por AUTOR ");
                mostrarCadena("\nNo se a encontrado ese AUTOR\n");
            }
            break;
        case 3:
            generarTitulo("Busqueda por GENERO ");
            asignarStringMsg(auxString,         "Ingrese GENERO a buscar en el registro");
            if(stringExisteEnArchivoLibro(auxString, "Genero"))
            {
                mostrarLibroPorGenero(auxString);
            }
            else
            {
                generarTitulo("Busqueda por GENERO ");
                mostrarCadena("\nNo se a encontrado ese GENERO\n");
            }
            break;
        case 4:
            generarTitulo("Busqueda por ISBN ");
            asignarValorMsg(&auxValor,         "Ingrese ISBN a buscar en el registro");
            if(isbnExisteEnArchivo(auxValor))
            {
                mostrarLibroPorISBN(auxValor);
            }
            else
            {
                generarTitulo("Busqueda por ISBN ");
                mostrarCadena("\nNo se a encontrado ese ISBN\n");
            }
            break;
        case 5:
            mostrarLibrosDisponibles();
            break;

        default:
            if(opcion != 0)
            {
                informarValorInvalido();
            }
            break;
        }
        system("pause");

    }
    while(!rangoValido(0,6,opcion) || opcion != 0);
}


void mostrarLibroPorTitulo(char titulo[])       //Muestra el libro segun su titulo
{
    stLibro auxLibro;
    FILE *archivo = fopen(archivoLibros, "rb");

    generarTitulo("Mostrar por TITULO");
    aplicarOffset(titulo);
    mostrarCadena(titulo);
    printf("\n\n");
    if(archivo != NULL)
    {
        fseek(archivo,0, SEEK_SET);

        while(fread(&auxLibro, sizeof(stLibro), 1,archivo)!=0)
        {
            if(strcasecmp(auxLibro.titulo, titulo) == 0)
            {
                mostrarstLibro(auxLibro);

            }
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoLibros();
    }



}

void mostrarLibroPorAutor(char autor[])     //muestra el libro segun su autor
{
    stLibro auxLibro;
    FILE *archivo = fopen(archivoLibros, "rb");

    generarTitulo("Mostrar por AUTOR");
    aplicarOffset(autor);
    mostrarCadena(autor);
    printf("\n\n");
    if(archivo != NULL)
    {
        fseek(archivo,0, SEEK_SET);

        while(fread(&auxLibro, sizeof(stLibro), 1,archivo)!=0)
        {
            if(strcasecmp(auxLibro.autor, autor) == 0)
            {
                mostrarstLibro(auxLibro);

            }
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoLibros();
    }
}

void mostrarLibroPorGenero(char genero[])       //muestra el libro segun su genero
{
    stLibro auxLibro;
    FILE *archivo = fopen(archivoLibros, "rb");

    generarTitulo("Mostrar por GENERO");
    aplicarOffset(genero);
    mostrarCadena(genero);
    printf("\n\n");
    if(archivo != NULL)
    {
        fseek(archivo,0, SEEK_SET);

        while(fread(&auxLibro, sizeof(stLibro), 1,archivo)!=0)
        {
            if(strcasecmp(auxLibro.genero, genero) == 0)
            {
                mostrarstLibro(auxLibro);
            }
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoLibros();
    }
}
void mostrarLibroPorISBN(int isbn)      //Muestra el libro segun su isbn
{
    stLibro auxLibro;
    FILE *archivo = fopen(archivoLibros, "rb");

    generarTitulo("Mostrar por ISBN");
    aplicarOffset("ISBN:    ");
    printf("ISBN: %i", isbn);
    printf("\n\n");
    if(archivo != NULL)
    {
        fseek(archivo,0, SEEK_SET);

        while(fread(&auxLibro, sizeof(stLibro), 1,archivo)!=0)
        {

            if(isbn == auxLibro.isbn)
            {
                mostrarstLibro(auxLibro);
            }
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoLibros();
    }

}

void mostrarLibrosDisponibles()     //muestra los libros disponibles
{
    stLibro auxLibro;
    FILE *archivo = fopen(archivoLibros, "rb");

    generarTitulo("Mostrar Disponibles");
    printf("\n");
    if(archivo != NULL)
    {
        fseek(archivo,0, SEEK_SET);

        while(fread(&auxLibro, sizeof(stLibro), 1,archivo)!=0)
        {
            if(auxLibro.disponible)
            {
                mostrarstLibro(auxLibro);
            }
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoLibros();
    }


}

void mostrarCantLibrosCargados()        //muestra la cantidad de libros cargados (numeralmente)
{
    FILE * archivo = fopen(archivoLibros,"rb");
    if(archivo!= NULL)
    {
        fseek(archivo, 0, SEEK_END);

        printf("| Libros cargados [ %i ]\n\n", ftell(archivo)/sizeof(stLibro));
        fclose(archivo);
    }
    else
    {
        mostrarCadena("Sin Libros\n\n");
    }

}
//------------------------------------------------------------------------------------------------------------
//                                      MODIFICAR PARAMETROS
//------------------------------------------------------------------------------------------------------------

void modificarLibroEnRegistro()     //permite la modificacion de libros
{
    FILE * archivo = fopen(archivoLibros, "r+b");
    int BDSM = 0;
    stLibro libroAModificar;

    if(archivo != NULL)
    {
        generarTitulo("Modificar LIBRO");
        mostrarCantLibrosCargados();
        asignarValorMsg(&BDSM, "Ingrese el ISBN del libro deseado");

        if(isbnExisteEnArchivo(BDSM))
        {
            fseek(archivo,0 , SEEK_SET);
            while(fread(&libroAModificar, sizeof(stLibro), 1, archivo) != 0 && (BDSM!= libroAModificar.isbn))
            {

                //you spin me round round baby right round

            }

            fseek(archivo, sizeof(stLibro) * -1, SEEK_CUR);
            mostrarstLibro(libroAModificar);

            modificarstLibro(&libroAModificar);
            fwrite(&libroAModificar, sizeof(stLibro), 1, archivo);
            fclose(archivo);
        }
        else
        {
            generarTitulo("Modificar LIBRO");
            aplicarOffset(">>   ISBN invalido en el contexto actual   <<");
            mostrarCadena(">>   ISBN invalido en el contexto actual   <<");
            printf("\n\n");
            system("pause");
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoLibros();
    }
}

void modificarstLibro(stLibro * libroAModificar)        //Pregunta que campo se desea editar y se edita mientras se ve por pantalla
{
    if(libroAModificar->disponible)
    {
        int opcion = 0, opcionB = 0;
        do
        {

            generarTitulo("Modificacion de LIBRO");
            mostrarCadena("Datos Actuales:");
            mostrarstLibro((*libroAModificar));
            printf("\n");
            crearLinea();
            printf("\n");
            mostrarCadena("1- Modificar TITULO");
            mostrarCadena("2- Modificar AUTOR");
            mostrarCadena("3- Modificar GENERO");
            mostrarCadena("4- Modificar ISBN");

            mostrarCadena("0- volver");

            printf("\n");

            asignarValorMsg(&opcion, "Ingrese su opcion");

            system("cls");
            generarTitulo("Modificacion de LIBRO");
            mostrarCadena("Datos Actuales:");
            mostrarstLibro((*libroAModificar));
            printf("\n");
            crearLinea();
            printf("\n");

            switch(opcion)
            {
            case 1:
                asignarStringMsg(libroAModificar->titulo,"Ingrese el nuevo TITULO");
                break;
            case 2:
                asignarStringMsg(libroAModificar->autor,"Ingrese el nuevo AUTOR");
                break;
            case 3:
                asignarStringMsg(libroAModificar->genero,"Ingrese la nueva GENERO");
                break;

            case 4:

                colorAmarillo();
                generarTitulo("ADVERTENCIA");
                mostrarCadena(">>  MODIFICAR ISBN SOLO SI SE REGISTRO DE MANERA INCORRECTA  <<");
                printf("\n\n\n");
                system("pause");

                generarTitulo("Modificacion de LIBRO");
                mostrarCadena("Datos Actuales:");
                mostrarstLibro((*libroAModificar));
                printf("\n\n");
                crearLinea();
                aplicarOffset(">>>  Desea cambiar el ISBN?    <<<");
                mostrarCadena(">>>  Desea cambiar el ISBN?    <<<");
                printf("\n");
                mostrarCadena("Cualquier valor diferente a 1 se tomara como 'NO'");
                printf("\n");
                mostrarCadena("1- Si");
                mostrarCadena("2- No");
                printf("\n");
                asignarValorMsg(&opcionB, "Ingrese su eleccion");

                colorBlanco();
                generarTitulo("Modificacion de LIBRO");
                mostrarCadena("Datos Actuales:\n");
                mostrarstLibro((*libroAModificar));
                printf("\n\n");
                crearLinea();


                if(opcionB == 1)
                {
                    int aux;
                    asignarValorMsg(&aux, "Ingrese el nuevo ISBN");
                    libroAModificar->isbn = aux;
                }
                else
                {
                    colorVerde();
                    aplicarOffset(">>>  Modificacion del ISBN cancelada    <<<");
                    mostrarCadena(">>>  Modificacion del ISBN cancelada    <<<");
                    printf("\n");
                    system("pause");
                    colorBlanco();
                }

                break;


            default:
                if(opcion != 0)
                {
                    informarValorInvalido();
                }
                break;
            }
        }
        while(!rangoValido(0,5,opcion) || opcion != 0);

    }
    else
    {

        generarTitulo("Modificacion de LIBRO");
        mostrarCadena("El libro solicitado no esta disponible\n");
        system("pause");
    }
}




//      INFORMAR POR PANTALLA

static void informarErrorAperturaArchivoLibros()      //Muestra un mensaje de error de apertura
{
    system("cls");
    generarTitulo("Error De Apertura");
    mostrarCadena("No se a podido encontrar/abrir el archivo de libros\n");
    system("pause");
}

//------------------------------------------------------------------------------------------------------------
//                              Eliminacion
//------------------------------------------------------------------------------------------------------------


void eliminarLibroDeArchivo()       //Elimina un libro del archivo mediante su ibsn
{
    int libroABorrar;


    generarTitulo("ELIMINAR LIBRO");
    mostrarCantLibrosCargados();
    asignarValorMsg(&libroABorrar, "Ingrese IBSN de libro a borrar");

    if(isbnExisteEnArchivo(libroABorrar))
    {
        moduloEliminarLibrodeArchivo(libroABorrar);

        system("pause");
    }
    else
    {

        generarTitulo("Valor Invalido");
        aplicarOffset("No se encuentra ese ID");
        mostrarCadena("No se encuentra ese ID\n");
        colorRojo();
        system("pause");
        colorBlanco();

    }

}


void moduloEliminarLibrodeArchivo(int isbm) {                 //Busca y elimina un libro por su isbm del archivo
    FILE * archivo = fopen(archivoLibros, "r+b");
    remove("librosTemp.bin");
    FILE * archTemp = fopen("librosTemp.bin", "w+b");

    stLibro aux;
    if (archivo != NULL && archTemp != NULL) {
        fseek(archivo, 0, SEEK_SET);

        while (fread(&aux, sizeof(stLibro), 1, archivo) == 1) {
            if (aux.isbn != isbm) {
                fwrite(&aux, sizeof(stLibro), 1, archTemp);
            }
        }

        fclose(archivo);
        fclose(archTemp);

        copiarArchivo("librosTemp.bin", archivoLibros);
        remove("librosTemp.bin");

    } else {
        if (archivo != NULL) {
            fclose(archivo);
        }
        if (archTemp != NULL) {
            fclose(archTemp);
        }
        informarErrorAperturaArchivoLibros();
    }

    eliminarArchivoSiVacio(archivoLibros);


}



int copiarArchivo(char archivoOrigen[], char archivoDestino[])      //Copia el archivo a en el archivo B
{
    FILE *origen = fopen(archivoOrigen, "rb");
    FILE *destino = fopen(archivoDestino, "wb");

    if (origen == NULL || destino == NULL)
    {
        return -1; // Error al abrir archivos
    }

    char buffer[1024];
    size_t bytesLeidos;

    while ((bytesLeidos = fread(buffer, 1, sizeof(buffer), origen)) > 0)
    {
        fwrite(buffer, 1, bytesLeidos, destino);
    }

    fclose(origen);
    fclose(destino);

    return 0; // Éxito
}

void eliminarArchivoSiVacio(char nombreArchivo[])       //Verifica si el archivo esta vacio, si lo esta, lo elimina
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo != NULL)
    {
        fseek(archivo, 0, SEEK_END);

        if (ftell(archivo) == 0)
        {
            fclose(archivo);
            remove(nombreArchivo);
        }
        else
        {
            fclose(archivo);

        }
    }
}

stLibro obtenerDatosLibroIBSM(int IBSM)
{
    FILE * archivo =fopen(archivoLibros, "rb");
    stLibro auxLib;

    if(archivo!= NULL)
    {
        while(fread(&auxLib, sizeof(stLibro), 1, archivo)!= 0 && IBSM != auxLib.isbn)
        {
        }
        fclose(archivo);
    }
    return auxLib;
}

