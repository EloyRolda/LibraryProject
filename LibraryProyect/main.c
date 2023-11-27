#include "prestamosLib.h"
int main()
{
    //system("mode con cols = 63 lines = 35");
    system("title El asombroso Trabajo Final de Eloy");

    mkdir("datos/");



    int opcion;
    do{
        generarTitulo("Programa Biblioteca");
        mostrarCadena("1- Gestionar Miembros");
        mostrarCadena("2- Gestionar Libros");
        mostrarCadena("3- Prestamos");
        mostrarCadena("0- Salir\n\n");
        asignarValorMsg(&opcion, "ingrese su opcion");
        switch(opcion)
        {
        case 1:
            menuMiembros();
            break;
        case 2:
            menuLibros();
            break;
        case 3:
            prestamoMenu();

            break;
        }

    } while(opcion!= 0);



//arbolNodoLibro * x = encontrarCategoria("A", NULL);
    return 0;
}
