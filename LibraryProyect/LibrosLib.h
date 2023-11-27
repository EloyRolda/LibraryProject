#include "dependenciasLib.h"

//  VARIABLES GLOBALES
#define archivoLibros "datos\\librosRegistrados.bin"
#define epsilon 0.0001

//      ESTRUCTURAS UTILIZADAS

typedef struct
{
    char titulo[largoCadena];
    char autor[largoCadena];
    char genero[largoCadena];
    int isbn;
    bool disponible;

} stLibro;

//      PROTOTIPADOS


//      MENUES
void menuLibros();                                              // Men� de gesti�n de libros
void menuMostrarPorCampoLibro();                                // Muestra seg�n el campo el/los libros pertinentes

//      CARGA DE DATOS
void agregarLibroArchivo(stLibro libroAgregable);               // Agrega un libro al archivo
void cargarstLibro(stLibro *nuevoLibro);                        // Carga todos los campos de un libro


//      MODIFICADORES

void modificarLibroEnRegistro();                                // Permite la modificaci�n de libros
void modificarstLibro(stLibro * libroAModificar);               // Pregunta qu� campo se desea editar y se edita mientras se ve por pantalla
void mostrarCantLibrosCargados();                               // Muestra la cantidad de libros cargados (numeralmente)

//      MOSTRAR INFORMACION POR PANTALLA

void mostrarLibroPorAutor(char autor[]);                        // Muestra el libro seg�n su autor
void mostrarLibroPorGenero(char genero[]);                      // Muestra el libro seg�n su g�nero
void mostrarLibroPorISBN(int isbn);                             // Muestra el libro seg�n su ISBN
void mostrarLibroPorTitulo(char titulo[]);                      // Muestra el libro seg�n su t�tulo
void mostrarLibrosDisponibles();                                // Muestra los libros disponibles
void mostrarRegistroLibros();                                   // Muestra el archivo libro por pantalla con todos sus elementos que est�n cargados

void mostrarstLibro(stLibro libro);                             // Muestra la estructura libro

static void informarErrorAperturaArchivoLibros();               // Muestra un mensaje de error de apertura

//      VALIDACIONES
bool isbnExisteEnArchivo(int isbnABuscar);                      // Verifica si un libro existe en el archivo por ISBN
bool stringExisteEnArchivoLibro(char cadena[], char tipo[]);    // Verifica si una cadena existe en el archivo de libros seg�n tipo (t�tulo, autor o g�nero)

//      ELIMINAR DE ARCHIVO

int copiarArchivo(char archivoOrigen[], char archivoDestino[]); // Copia el contenido del archivo origen al archivo destino
void eliminarArchivoSiVacio(char nombreArchivo[]);              // Verifica si el archivo est� vac�o y lo elimina en ese caso
void eliminarLibroDeArchivo();                                  // Elimina un libro del archivo mediante su ISBN
void moduloEliminarLibrodeArchivo(int isbm);                    // Busca y elimina un libro por su ISBN del archivo
