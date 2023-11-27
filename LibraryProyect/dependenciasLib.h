#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"
#include "time.h"
#include "windows.h"
#include "math.h"


//-------------------------------------------------------------------------------------------------------------------
//                          Librerias de Eloy
//-------------------------------------------------------------------------------------------------------------------

//  CONSTANTES
#define largoCadena 30

//      FUNCIONES DE MUSTRA POR PANTALLA
void generarTitulo(char titulo[]);                      // Crea un título "distintivo"
void mostrarCadena(char arr[]);                         // Muestra por parámetro una cadena de caracteres
void mostrarCadenaMsg(char msg[], char arr[]);          // Muestra por parámetro una cadena de caracteres: dato
void aplicarOffset(char texto[]);                       // Aplica tabulaciones según el largo del texto
void debug(char msg[]);                                 // Se utiliza para ver los procesos del programa
void crearLinea();                                      // Imprime una línea en pantalla
void informarValorInvalido();                           // Informa que el valor ingresado no es válido


//      FUNCIONES DE CARGA DE VALORES
void asignarStringMsg(char cadena[], char mensaje[]);   // Recibe por referencia un string y un mensaje al usuario
void asignarString(char cadena[]);                      // Recibe por referencia un string para ser cargado
void asignarValorMsg(int *valor, char mensaje[]);       // Recibe por referencia un valor y un mensaje al usuario
void asignarValor(int *valor);                          // Recibe por referencia un valor para ser cargado


//      FUNCIONES ESTETICAS
void colorVerde();                                      // Cambia el color de la consola a verde
void colorRojo();                                       // Cambia el color de la consola a rojo
void colorAmarillo();                                   // Cambia el color de la consola a amarillo
void colorBlanco();                                     // Cambia el color de la consola a blanco

//      FUNCIONES DE VALIDACION
bool rangoValido(int min, int max, int opcion);         // Valida que la opción esté entre el mínimo y el máximo
bool preguntarConfirmacion(char msg[]);                 // Recibe un string para retornar un booleano con la respuesta

