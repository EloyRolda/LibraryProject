#include "miembrosLib.h"


//      GLOBALES
#define archivoPrestamos "datos\\librosPrestados.bin"
#define tiempoPrestamo 7   //7 DIAS DE PRESTAMO
#define maxPrestamos 3
#define cargoPorDeuda 150   //pesos argentino

typedef struct{
int prestamo[3];    //Dia Mes A�o
int vencimiento[3]; //Dia Mes A�o
}stFecha;

typedef struct{
stMiembro miembro;
stLibro prestado;
stFecha fecha;
stMiembro reservado;
int retrazo;
}stPrestamo;

typedef struct{

stLibro libro;

struct arbolNodoLibro *menor;
struct arbolNodoLibro *mayor;


}arbolNodoLibro;

typedef struct
{
    char categoria[largoCadena];
    arbolNodoLibro * raiz;
    struct nodoLibro *  sig;
} nodoLibro;

//PROTOTIPADOS

void prestamoMenu();

void mostrarCategorias(nodoLibro *lista);
stPrestamo obtenerReservaLibro(int ibsm);
void imprimirPrestamo(stPrestamo prestamo);

//-------------------------------------------------------------------------
//                                 ARBOLES
//-------------------------------------------------------------------------

arbolNodoLibro * inicArbolNodo();  // Inicializa un �rbol de libros y retorna NULL
void verArbolCategoria(char categoria[], nodoLibro * lista);  // Muestra un �rbol de una categor�a espec�fica
void imprimirArbol(arbolNodoLibro * raiz);  // Imprime un �rbol de libros
arbolNodoLibro * crearNodoArbolLibro(stLibro dato);  // Crea un nodo de un �rbol de libros con los datos y lo retorna
arbolNodoLibro * insertarLibroEnArbol(arbolNodoLibro * nuevoLibro, arbolNodoLibro * raiz);  // Inserta un libro en un �rbol
bool nodoArbolLibroExiste(int ibsm, arbolNodoLibro *raiz);  // Verifica si un nodo de libro existe en un �rbol
arbolNodoLibro * encontrarCategoria(char categoria[], nodoLibro * lista);  // Busca una categor�a en una lista y retorna su ra�z
arbolNodoLibro * obtenerLibroPorISBN(int isbn, arbolNodoLibro * raiz);  // Retorna la direcci�n de memoria del libro por su ISBN



//-------------------------------------------------------------------------
//                                 NODOS | LISTA
//-------------------------------------------------------------------------

nodoLibro * pasarLibrosArbol(nodoLibro * lista);  // Transfiere los libros a un �rbol
bool categoriaExisteEnLista(char categoria[], nodoLibro * lista);  // Verifica si una categor�a existe en una lista
nodoLibro * crearCategoria(char categoria[], nodoLibro * lista);  // Crea una categor�a y la agrega al final de la lista
nodoLibro * crearNodoLibroParametro(char categoria[]);  // Crea un nodoLibro con la categor�a cargada
nodoLibro * inicNodoLibro();  // Inicializa un nodoLibro y retorna NULL
arbolNodoLibro * cargarLibroArbol(arbolNodoLibro * nuevoLibro, arbolNodoLibro * raiz);  // Carga un libro en un �rbol de libros
nodoLibro *prestarLibro(nodoLibro *lista);
//-------------------------------------------------------------------------
//                                  ARCHIVOS
//-------------------------------------------------------------------------

void agregarPrestamoArchivo(stPrestamo prestamo);  // Agrega un pr�stamo al archivo
bool prestamoExiste(int id);  // Verifica si un pr�stamo existe
void mostrarArchivoPrestamos();  // Muestra el archivo de pr�stamos
void purgarPrestamos();  // Elimina los pr�stamos no v�lidos

//-------------------------------------------------------------------------
//                                  MIEMBROS
//-------------------------------------------------------------------------
void cambiarEstadoMiembro(int id);  // Cambia el estado de un miembro

//-------------------------------------------------------------------------
//                                  LIBROS
//-------------------------------------------------------------------------

void cambiarEstadoLibro(stLibro libro);  // Cambia el estado de un libro
//-------------------------------------------------------------------------
//                                  FECHAS
//-------------------------------------------------------------------------

void cargarFecha(stFecha *fecha);  // Carga la fecha actual
void extenderPlazo(stFecha *fecha);  // Extiende el plazo de una fecha

//---------------------------------------------------------------------------------
//                                  DEVOLUCIONES
//---------------------------------------------------------------------------------

void devolverLibro();  // Devuelve un libro
void eliminarDeudaMiembro(int ID);  // Elimina la deuda de un miembro
void eliminarDeudorArchivo(int ID);  // Elimina un deudor del archivo
