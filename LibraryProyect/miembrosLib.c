#include "dependenciasLib.h"
#include "miembrosLib.h"

void menuMiembros()     //llamada a las funciones pertinentes para gestionar miembros y archivos de mimbros
{

    int opcion = 0;


    do
    {
        generarTitulo("Menu Gestion miembros");

        mostrarCadena("1- Registrar miembro");
        mostrarCadena("2- Ver registro de miembros");
        mostrarCadena("3- Ver miembro deseado");
        mostrarCadena("4- Modificar Miembro");
        mostrarCadena("5- Purgar Miembros NO ACTIVOS");
        mostrarCadena("0- Volver");
        printf("\n");

        asignarValorMsg(&opcion, "Ingrese su opcion");

        switch(opcion)
        {
        case 1:
            registrarMiembro();
            break;
        case 2:
            mostrarRegistroMiembros();
            break;
        case 3:
            menuMostrarPorCampo();
            break;
        case 4:
            modificarMiembroEnRegistro();
            break;
        case 5:
            generarTitulo("CONFIRMACION");
            if(preguntarConfirmacion("Desea eliminar los miembros NO ACTIVOS del registro?\nEsta accion no se puede deshacer y puede generar errores en el programa")){
            purgarArchivoMiembros();
            system("pause");
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

void registrarMiembro()         //Se carga una stMiembro, se busca si existe en el archivo y en caso de que no, se carga
{

    stMiembro nuevoMiembro;
    cargarstMiembro(&nuevoMiembro);

    if(miembroExisteEnArchivo(nuevoMiembro))
    {
        generarTitulo("Usuario Ya Existente");  //Si el usuario existe (DNI YA CARGADO EN OTRO MIEMBRO)
        colorRojo();                            //Muestra el Miembro con ese DNI para que pueda revisarlo/llamar, etc
        mostrarMiembroDNI(nuevoMiembro.dni);
        system("pause");
    }
    else
    {
        generarTitulo("Miembro Agregado");      //Si el usuario NO existe (DNI SIN COINCIDENCIAS)
        agregarMiembroArchivo(nuevoMiembro);    //Carga la estructura al archivo y avisa que fue cargado
        colorVerde();
        system("pause");
    }
    colorBlanco();

}

void cargarstMiembro(stMiembro *nuevoMiembro)   //Carga todos los campos de stMiembros por el usuario, exepto ID
{
    generarTitulo("Nuevo Miembro");
    nuevoMiembro->id = obtenerUltimoIdMiembros() + 1;
    asignarStringMsg(nuevoMiembro->nombre,      "Ingrese el nombre");
    asignarStringMsg(nuevoMiembro->apellido,    "Ingrese el apellido");
    asignarStringMsg(nuevoMiembro->direccion,   "Ingrese la direccion");
    asignarStringMsg(nuevoMiembro->numero,      "Ingrese numero telefonico");
    asignarStringMsg(nuevoMiembro->dni,         "Ingrese numero de documento");

    //Genera el nombre del archivo [[[ Nombre_Apellido_historialPrestamo.bin ]]]
    char nombreArchivo[largoCadena];
    strcpy(nombreArchivo, nuevoMiembro->nombre);
    strcat(nombreArchivo, "_");
    strcat(nombreArchivo, nuevoMiembro->apellido);
    strcat(nombreArchivo, "_historialPrestamo.bin");
    strcpy(nuevoMiembro->archPrestamo, nombreArchivo);
    //Fin generacion nombre del archivo


    nuevoMiembro->deudas = 0.00000;
    nuevoMiembro->activo = true;


}

int obtenerUltimoIdMiembros()                   //Busca y retorna el ultimo id del archivo de miembros
{
    FILE* archivo = fopen(archivoMiembros, "rb");
    int ultimo = 0;
    stMiembro miembroAux;
    if(archivo != NULL)
    {
        fseek(archivo, sizeof(stMiembro) * -1, SEEK_END);
        fread(&miembroAux, sizeof(stMiembro),1, archivo);
        ultimo = miembroAux.id;
        fclose(archivo);
    }

    return ultimo;
}

void mostrarstMiembro(stMiembro miembroMostrable) //Muestra el miembro recibido por pantalla
{


    printf("| ID:           %i\n", miembroMostrable.id);
    if(miembroMostrable.activo)
    {
        mostrarCadena("| Estado:       ACTIVO");
    }
    else
    {

        mostrarCadena("| Estado:       NO ACTIVO");
    }
    mostrarCadenaMsg("| Nombre:       ",    miembroMostrable.nombre);
    mostrarCadenaMsg("| Apellido:     ",    miembroMostrable.apellido);
    mostrarCadenaMsg("| Direccion:    ",    miembroMostrable.direccion);
    mostrarCadenaMsg("| Numero:       ",    miembroMostrable.numero);
    mostrarCadenaMsg("| DNI:          ",    miembroMostrable.dni);

    printf("| \n");

    if(miembroMostrable.deudas > 0)
    {
        mostrarCajitaDeuda(miembroMostrable.deudas );
    }
    else
    {
        printf("| DEUDOR:       NO\n");
    }

    //mostrarHistorialPrestamos(miembroMostrable.archPrestamo);
}

static void mostrarCajitaDeuda(float monto)         //Muestra su deuda
{
    //printf("|------------------------------|\n");
    printf("| DEUDOR:       SI\n");
    printf("| MONTO:        %.2f\n", monto);
    //printf("|------------------------------|\n");
}

void agregarMiembroArchivo(stMiembro miembro)   //Carga el miembro recibido por parametro al final del archivo
{
    FILE * archivo = fopen(archivoMiembros, "ab");
    if(archivo != NULL)
    {
        fseek(archivo, sizeof(stMiembro), SEEK_END);
        fwrite(&miembro, sizeof(stMiembro), 1, archivo);
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoMiembros();
    }
}

bool miembroExisteEnArchivo(stMiembro miembroAVerificar)            //Verifica si nuestro amigo esta en el archivo
{
    bool existe = false;

    stMiembro miembroAux;

    FILE * archivo = fopen(archivoMiembros, "rb");
    if(archivo != NULL)
    {

        fseek(archivo, 0, SEEK_SET);

        while(fread(&miembroAux, sizeof(stMiembro), 1, archivo)>0 && !existe)
        {

            if(strcmp(miembroAVerificar.dni, miembroAux.dni) == 0)
            {
                existe = true;
            }
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoMiembros();
    }
    return existe;
}

bool verificarMiembroID(int idMiembro){     //Verifica si el ID recibido existe en el archivo
bool existe = false;

    stMiembro miembroAux;

    FILE * archivo = fopen(archivoMiembros, "rb");
    if(archivo != NULL)
    {

        fseek(archivo, 0, SEEK_SET);

        while(fread(&miembroAux, sizeof(stMiembro), 1, archivo)>0 && !existe)
        {

            if(idMiembro == miembroAux.id)
            {
                existe = true;
            }
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoMiembros();
    }
    return existe;

}


static void informarErrorAperturaArchivoMiembros()      //Muestra un mensaje de error de apertura
{
    system("cls");
    generarTitulo("Error De Apertura");
    mostrarCadena("No se a podido encontrar/abrir el archivo de miembros\n");
    system("pause");
}

void mostrarArchivoMiembros()               //Muestra por pantalla todos los miembros registrados en el archivo
{

    stMiembro miembroAux;
    FILE * archivo = fopen(archivoMiembros, "rb");
    if(archivo != NULL)
    {
        while(fread(&miembroAux, sizeof(stMiembro), 1, archivo) != 0)
        {
            mostrarstMiembro(miembroAux);
            printf("\n\n");
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoMiembros();
    }
}
//Mostrar Por campo especifico


void mostrarRegistroMiembros()      //Muestra todos los Miembros registrados
{
    generarTitulo("Miembro de Biblioteca Archivo");
    mostrarArchivoMiembros();
    system("pause");
}

void mostrarMiembroID(int id)       //Muestra por id (deberia mostrar solamente 1)
{

    stMiembro miembroAux;
    FILE * archivo = fopen(archivoMiembros, "rb");

    generarTitulo("Usuarios cargado con ese ID");
    aplicarOffset("a"); //un numero es un caracter.
    if(id >=1 && id <= (obtenerUltimoIdMiembros()))
    {
        printf("[ %i ]\n\n", id);

        if(archivo != NULL)
        {
            while(fread(&miembroAux, sizeof(stMiembro), 1, archivo) != 0)
            {
                if(id == miembroAux.id)
                {
                    mostrarstMiembro(miembroAux);
                }
                printf("\n");
            }
            fclose(archivo);
        }
        else
        {
            informarErrorAperturaArchivoMiembros();
        }
    }
    else
    {
        generarTitulo("ID INVALIDO");
        printf("3107\n");
        Sleep(150);
        colorRojo();
        generarTitulo("ID INVALIDO");
        mostrarCadena("INGRESE ID VALIDO POR ENCIMA DE 1");
        colorBlanco();
    }
}
void mostrarMiembroDNI(char dni[])  //Muestra por DNI (deberia mostrar solamente 1)
{
    stMiembro miembroAux;
    FILE * archivo = fopen(archivoMiembros, "rb");

    generarTitulo("Usuarios cargado con ese DNI");
    aplicarOffset(dni);
    printf("[ %s ]\n\n", dni);


    if(archivo != NULL)
    {
        while(fread(&miembroAux, sizeof(stMiembro), 1, archivo) != 0)
        {
            if(strcmp(dni, miembroAux.dni)== 0)
            {
                mostrarstMiembro(miembroAux);
            }
            printf("\n");
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoMiembros();
    }
}
void mostrarMiembroNombre(char nombre[])    //Muestra por Nombre (deberia mostrar todas las coincidencias)
{

    stMiembro miembroAux;
    FILE * archivo = fopen(archivoMiembros, "rb");
    generarTitulo("Usuarios cargados con ese nombre");
    aplicarOffset(nombre);
    printf("[ %s ]\n\n", nombre);

    if(archivo != NULL)
    {
        while(fread(&miembroAux, sizeof(stMiembro), 1, archivo) != 0)
        {
            if(strcmp(nombre, miembroAux.nombre)== 0)
            {
                mostrarstMiembro(miembroAux);
            }
            printf("\n");
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoMiembros();
    }
}

void mostrarMiembroApellido(char apellido[])     //Muestra por Apellido (deberia mostrar todas las coincidencias)
{

    stMiembro miembroAux;
    FILE * archivo = fopen(archivoMiembros, "rb");
    generarTitulo("Usuarios cargados con ese apellido");
    aplicarOffset(apellido);
    printf("[ %s ]\n\n", apellido);

    if(archivo != NULL)
    {
        while(fread(&miembroAux, sizeof(stMiembro), 1, archivo) != 0)
        {
            if(strcmp(apellido, miembroAux.apellido)== 0)
            {
                mostrarstMiembro(miembroAux);
            }
            printf("\n");
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoMiembros();
    }
}

//Fin mostrar miembro por campo especifico

void menuMostrarPorCampo()          //Menu para mostrar un/unos miembros segun un campo especifico, id, dni, nombre, apellido
{
    int opcion = 0;
    char auxString[largoCadena];
    int auxValor = 0;

    do
    {
        generarTitulo("Mostrar Por Campo");
        fflush(stdin);

        //Opciones
        mostrarCadena("1- Mostrar por ID");
        mostrarCadena("2- Mostrar por DNI");
        mostrarCadena("3- Mostrar por Nombre");
        mostrarCadena("4- Mostrar por Apellido");
        mostrarCadena("0- Volver");


        //FinOpciones
        printf("\n");
        asignarValorMsg(&opcion, "Ingrese su opcion");
        switch(opcion)
        {
        case 1:
            generarTitulo("Busqueda por ID");
            asignarValorMsg(&auxValor,           "Ingrese ID a buscar en el registro");
            if(verificarMiembroID(auxValor)){
            mostrarMiembroID(auxValor);
            }else{
            generarTitulo("Busqueda por ID");
            mostrarCadena("\nNo se a encontrado ese usuario, revise el registro e intente nuevamente\n");
            }

            break;
        case 2:
            generarTitulo("Busqueda por DNI");
            asignarStringMsg(auxString,         "Ingrese DNI a buscar en el registro");
            mostrarMiembroDNI(auxString);
            break;
        case 3:
            generarTitulo("Busqueda por NOMBRE");
            asignarStringMsg(auxString,         "Ingrese NOMBRE a buscar en el registro");
            mostrarMiembroNombre(auxString);
            break;
        case 4:
            generarTitulo("Busqueda por APELLIDO");
            asignarStringMsg(auxString,         "Ingrese NOMBRE a buscar en el registro");
            mostrarMiembroApellido(auxString);
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
    while(!rangoValido(0,4,opcion) && opcion != 0);
}



void modificarMiembroEnRegistro()       //Abre el registro pertinente obtiene el usuario y llama a la modificacion de sus campos
{

    FILE * archivo = fopen(archivoMiembros, "r+b");
    int ID = 0;
    stMiembro miembroAModificar;
    if(archivo != NULL)
    {

        generarTitulo("Modificar Miembro");
        mostrarCantMiembrosCargados();
        asignarValorMsg(&ID, "Ingrese ID del miembro a Modificar");
        if(verificarMiembroID(ID))
        {


            while(fread(&miembroAModificar, sizeof(stMiembro), 1, archivo) != 0 && ID != miembroAModificar.id){


            }
            fseek(archivo, sizeof(stMiembro) * -1, SEEK_CUR);
            mostrarstMiembro(miembroAModificar);

            modificarstMiembro(&miembroAModificar);
            fwrite(&miembroAModificar, sizeof(stMiembro), 1, archivo);

            fclose(archivo);
        }
        else
        {
            generarTitulo("Modificar Miembro");
            aplicarOffset(">>   ID invalido en el contexto actual   <<");
            mostrarCadena(">>   ID invalido en el contexto actual   <<");
            printf("\n\n");
            system("pause");
        }
        fclose(archivo);
    }
    else
    {
        informarErrorAperturaArchivoMiembros();
    }
}

void modificarstMiembro(stMiembro * miembroAModificar)  //Modifica los campos de la estructura miembro, exeptuando los autoasignados como deuda e ID
{

    int opcion = 0, opcionB = 0;
    do
    {

        generarTitulo("Modificacion de miembro");
        mostrarCadena("Datos Actuales:");
        mostrarstMiembro((*miembroAModificar));
        printf("\n");
        crearLinea();
        printf("\n");
        mostrarCadena("1- Modificar NOMBRE");
        mostrarCadena("2- Modificar APELLIDO");
        mostrarCadena("3- Modificar DIRECCION");
        mostrarCadena("4- Modificar NUMERO");
        mostrarCadena("5- Modificar DNI");
        mostrarCadena("0- Volver");
        if(miembroAModificar->activo)
        {
            mostrarCadena("6- Dar de BAJA");
        }
        else
        {
            mostrarCadena("6- Dar de ALTA");

        }
        mostrarCadena("0- Volver");
        printf("\n");

        asignarValorMsg(&opcion, "Ingrese su opcion");

        system("cls");
        generarTitulo("Modificacion de miembro");
        mostrarCadena("Datos Actuales:");
        mostrarstMiembro((*miembroAModificar));
        printf("\n");
        crearLinea();
        printf("\n");

        switch(opcion)
        {
        case 1:
            asignarStringMsg(miembroAModificar->nombre,"Ingrese el nuevo NOMBRE");
            break;
        case 2:
            asignarStringMsg(miembroAModificar->apellido,"Ingrese el nuevo APELLIDO");
            break;
        case 3:
            asignarStringMsg(miembroAModificar->direccion,"Ingrese la nueva DIRECCION");
            break;
        case 4:
            asignarStringMsg(miembroAModificar->numero,"Ingrese el nuevo NUMERO");
            break;
        case 5:

            colorAmarillo();
            generarTitulo("ADVERTENCIA");
            mostrarCadena(">>  MODIFICAR DNI SOLO SI SE REGISTRO DE MANERA INCORRECTA  <<");
            printf("\n\n\n");
            system("pause");

            generarTitulo("Modificacion de miembro");
            mostrarCadena("Datos Actuales:");
            mostrarstMiembro((*miembroAModificar));
            printf("\n\n");
            crearLinea();
            aplicarOffset(">>>  Desea cambiar el DNI?    <<<");
            mostrarCadena(">>>  Desea cambiar el DNI?    <<<");
            printf("\n");
            mostrarCadena("Cualquier valor diferente a 1 se tomara como 'NO'");
            printf("\n");
            mostrarCadena("1- Si");
            mostrarCadena("2- No");
            printf("\n");
            asignarValorMsg(&opcionB, "Ingrese su eleccion");

            colorBlanco();
            generarTitulo("Modificacion de miembro");
            mostrarCadena("Datos Actuales:");
            mostrarstMiembro((*miembroAModificar));
            printf("\n\n");
            crearLinea();
            if(opcionB == 1)
            {
                asignarStringMsg(miembroAModificar->dni,"Ingrese el DNI corregido");
            }
            else
            {
                colorVerde();
                aplicarOffset(">>>  Modificacion del DNI cancelada    <<<");
                mostrarCadena(">>>  Modificacion del DNI cancelada    <<<");
                printf("\n");
                system("pause");
                colorBlanco();
            }

            break;
        case 6:
            if(miembroAModificar->deudas == 0)
            {
                if(miembroAModificar->activo)
                {
                    miembroAModificar->activo = false;
                }
                else
                {
                    miembroAModificar->activo = true;
                }
            }
            else
            {
                generarTitulo("Modificacion de miembro");
                aplicarOffset(">>>  No se pudo dar de bajas, tiene deudas pendientes    <<<");
                mostrarCadena(">>>  No se pudo dar de bajas, tiene deudas pendientes    <<<");
                printf("\n");
                system("pause");
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

void mostrarCantMiembrosCargados()          //Imprime por pantalla la cantidad de miembros cargados.
{
    FILE * archivo = fopen(archivoMiembros,"rb");
    if(archivo!= NULL)
    {
        fseek(archivo, 0, SEEK_END);

        printf("| Miembros cargados [ %i ]\n\n", ftell(archivo)/sizeof(stMiembro));
        fclose(archivo);
    }
    else
    {
        mostrarCadena("Sin Miembros\n\n");
    }

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                              Listas Momento
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void purgarArchivoMiembros()        //Pasa los miembros "activos" a una lista simple, luego borra el archivo para pasar todos los elementos de la lista a el sin los usuarios inactivos
{
    nodoMiembro * lista = inicListMiembro();

    int miembrosCargados = obtenerUltimoIdMiembros();
    stMiembro auxMiembro;
    FILE * archivo = fopen(archivoMiembros, "r+b");

    if(archivo != NULL)
    {

        fseek(archivo, 0, SEEK_SET);

        while(fread(&auxMiembro, sizeof(stMiembro), 1, archivo)!= 0)
        {
            if(auxMiembro.activo)
            {
                lista = agregarAlistaMiembro(lista, crearNodoMiembro(auxMiembro));
            }
        }

        fclose(archivo);
        remove(archivoMiembros);
    }

    else
    {
        informarErrorAperturaArchivoMiembros();
    }


    pasarListaMiembrosAArchivo(lista);



}

nodoMiembro * inicListMiembro(){        //Retorna NULL para los nodoMiembro
return NULL;
}

nodoMiembro * crearNodoMiembro(stMiembro datosMiembro)      //Recibe una structura, crea un nodo y lo retorna cargado
{
    nodoMiembro * nuevoNodoMiembro = malloc(sizeof(nodoMiembro));
    nuevoNodoMiembro->sig = inicListMiembro();
    nuevoNodoMiembro->miembro = datosMiembro;
    return nuevoNodoMiembro;
}

nodoMiembro * agregarAlistaMiembro(nodoMiembro * lista, nodoMiembro * nuevoMiembroNodo) //Agrega el nodo a lo ultimo de la lista
{

    nodoMiembro * seguidora = inicListMiembro();

    if(lista != NULL)
    {
        seguidora = lista;

        while(seguidora->sig != NULL)
        {
            seguidora = seguidora->sig;
        }
        seguidora->sig = nuevoMiembroNodo;

    }
    else
    {

        lista = nuevoMiembroNodo;

    }
    return lista;
}

void mostrarListaMiembros(nodoMiembro * lista)      //Muestra recursivamente la lista [No usado para el usuario]
{                                                   //Se uso para corroborar la funcionalidad del programa
    if(lista != NULL)
    {
        crearLinea();
        printf("Actual: %p\n", lista);
        mostrarstMiembro(lista->miembro);
        printf("Siguiente: %p\n", lista->sig);
        mostrarListaMiembros(lista->sig);
        crearLinea();
    system("pause");

    }
}

void pasarListaMiembrosAArchivo(nodoMiembro * lista)        //Pasa los nodo miembros al archivo de manera ordenada
{
    if(lista != NULL)
    {
        agregarMiembroArchivo(lista->miembro);
        pasarListaMiembrosAArchivo(lista->sig);
        free(lista);
    }
}

void informarErrorIDNoValido(){

  generarTitulo("PRESTAMOS Y DEVOLUCIONES");
        colorRojo();
        mostrarCadena("ID NO ENCONTRADO EN EL REGISTRO USUARIOS");
        printf("\n");
        system("pause");
        colorBlanco();


}

stMiembro obtenerMiembro(int id)
{

    FILE *archivo = fopen(archivoMiembros, "rb");
    stMiembro aux;
    if(archivo != NULL)
    {

        while(fread(&aux, sizeof(stMiembro), 1, archivo) !=0 && id != aux.id)
        {

        }
        fclose(archivo);
    }

    return aux;
}

