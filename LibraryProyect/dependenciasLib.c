#include "dependenciasLib.h"


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//                                              Pasar a otra libreria
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//--------------------      MUESTRAS POR PANTALLA       --------------------
void generarTitulo(char titulo[])                       //Crea un titulo "distintivo"
{

    system("cls");
    printf("--------------------------------------------------------------\n");
    aplicarOffset(titulo);
    printf("%s\n", titulo);
    printf("--------------------------------------------------------------\n\n");

}


void mostrarCadena(char arr[])                          //Muestra por parametro una cadena de caracteres
{
    printf("%s\n", arr);
}
void mostrarCadenaMsg(char msg[], char arr[])           //Muestra por parametro una cadena de caracteres
{
    printf("%s", msg);
    mostrarCadena(arr);
}

void aplicarOffset(char texto[])        //aplica tabulaciones segun el largo del texto (gotoxy salio del chat)
{
    int largoTitulo = strlen(texto);
    if(largoTitulo<5)
    {
        printf("                            ");
    }
    else if(largoTitulo >= 6 &&largoTitulo < 15)
    {
        printf("                       ");
    }
    else if(largoTitulo >= 16 &&largoTitulo< 25)
    {
        printf("                   ");
    }
    else if(largoTitulo >= 26 &&largoTitulo < 35)
    {
        printf("              ");
    }
    else
    {
        printf("        ");
    }

}

void debug(char msg[])          //Se utiliza para ver los procesos del programa imprimiento por pantalla debug."mensaje"
{
    printf("---------------------------\n");
    printf("debug.");
    printf("%s\n", msg);
    printf("---------------------------\n");
    system("pause");

}

void crearLinea()       //Imprime una linea en pantalla
{

    printf("--------------------------------------------------------------\n");
}

void informarValorInvalido()        //Informa que el valor ingresado no es valido (se utiliza para los menues)
{
    generarTitulo("Valor Invalido");
    colorRojo();
    mostrarCadena("Ingrese Un valor Valido entre las Opciones disponibles");
    printf("\n");
    system("pause");
    colorBlanco();
}

//--------------------      FUNCIONES DE CARGA DE VALORES       --------------------

void asignarStringMsg(char cadena[], char mensaje[])    //Recibe por referencia un string y un mensaje al usuario
{
    //Imprime el mensaje y carga el string llamando a "asignarString"
    printf("* %s\n>: ", mensaje);
    asignarString(cadena);
}

void asignarString(char cadena[])                       //Recibe por referencia un string para ser cargado
{
    fflush(stdin);
    gets(cadena);
}
void asignarValorMsg(int *valor, char mensaje[])    //Recibe por referencia un string y un mensaje al usuario
{
    //Imprime el mensaje y carga el Valor llamando a "asignarValor"
    printf("* %s\n>: ", mensaje);
    asignarValor(valor);
}

void asignarValor(int *valor)                     //Recibe por referencia un valor para ser cargado
{
    int aux;
    fflush(stdin);
    scanf("%i", &(*valor));

}

//--------------------      CAMBIO DE COLOR CONSOLA (ESTETICO)       --------------------
void colorVerde()
{

    system("COLOR A");

}
void colorRojo()
{

    system("COLOR 4");

}

void colorAmarillo()
{

    system("COLOR 6");
}
void colorBlanco()
{
    system("COLOR 7");
}

//--------------------      FUNCIONES DE VALIDACION       --------------------

bool rangoValido(int min, int max, int opcion)    //Recibe 3 Parametros, para hacer validar que la opcion este entre el max
{
    //Y el minimo

    return(opcion >= min && opcion <= max);
}

bool preguntarConfirmacion(char msg[])      //Recibe un string, para retornar un boleeano con la respuesta
{
    int valor = 0;
    mostrarCadena(msg);
    printf("\n");
    mostrarCadena("1- SI");
    mostrarCadena("2- NO");

    printf(">: ");
    asignarValor(&valor);


    return (valor == 1);
}

