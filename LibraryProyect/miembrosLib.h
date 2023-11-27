#include "LibrosLib.h"
//-------------------------------------------------------------------------------------------------------------------
//                          LIBRERIA DE MIEMBROS
//-------------------------------------------------------------------------------------------------------------------

//      CONSTANTES DEFINIDAS
#define archivoMiembros "datos\\miembrosRegistrados.bin" //Archivo donde se cargaran los miembros


//      ESTRUCTURAS UTILIZADAS

typedef struct
{
    int id;                             //ID, nunca puede ser 0
    char nombre[largoCadena];           //Nombre y apellido
    char apellido[largoCadena];
    char direccion[largoCadena];        //Direccion fisica, se asume que vive en la misma ciudad
    char numero[largoCadena];           //Numero Telefonico
    char dni[largoCadena];              //Documento nacional de identidad
    char archPrestamo[largoCadena];     //Nombre del archivo binario donde se cargara el historial
    float deudas;                       //Valor total de deudas a pagar
    bool activo;                        //Activo = false, si el usuario a sido "eliminado"
} stMiembro;

typedef struct
{
    stMiembro miembro;                      //El stMiembro
    struct nodoMiembro * sig;               //Puntero al siguiente NODO
} nodoMiembro;

//      PROTOTIPADOS

// FUNCIONES DE GESTIÓN DE MIEMBROS

//      MENUES
void menuMiembros();                                        // Llamada a funciones para gestionar miembros y archivos
void menuMostrarPorCampo();                                 // Menu para mostrar miembros según un campo específico

//      CARGA DE DATOS Y REGISTRO
void registrarMiembro();                                    // Carga y verifica si el miembro ya existe en el archivo
void cargarstMiembro(stMiembro *nuevoMiembro);              // Carga todos los campos de stMiembros por el usuario
stMiembro obtenerMiembro(int id);
//      MODIFICADORES DE EXISTENTES
void modificarMiembroEnRegistro();                          // Obtiene el usuario y llama a la modificación de sus campos
void modificarstMiembro(stMiembro *miembroAModificar);      // Modifica los campos de la estructura miembro

//      AUXILIARES
int obtenerUltimoIdMiembros();                              // Busca y retorna el último ID del archivo de miembros
void agregarMiembroArchivo(stMiembro miembro);              // Carga el miembro recibido al final del archivo

//      VALIDACIONES DE ESTADO
bool miembroExisteEnArchivo(stMiembro miembroAVerificar);   // Verifica si el miembro está en el archivo
bool verificarMiembroID(int idMiembro);                     // Verifica si el ID recibido existe en el archivo

//      MUESTRAS POR PANTALLA DE INFORMACION
    //de estructuras
void mostrarArchivoMiembros();                              // Muestra por pantalla todos los miembros registrados
void mostrarMiembroID(int id);                              // Muestra por ID (debería mostrar solamente 1)
void mostrarMiembroDNI(char dni[]);                         // Muestra por DNI (debería mostrar solamente 1)
void mostrarMiembroNombre(char nombre[]);                   // Muestra por Nombre (debería mostrar todas las coincidencias)
void mostrarMiembroApellido(char apellido[]);               // Muestra por Apellido (debería mostrar todas las coincidencias)
void mostrarCantMiembrosCargados();                         // Imprime por pantalla la cantidad de miembros cargados
void mostrarstMiembro(stMiembro miembroMostrable);          // Muestra el miembro recibido por pantalla
static void mostrarCajitaDeuda(float monto);                       // Muestra la deuda del miembro

    //de apertura de archivos
static void informarErrorAperturaArchivoMiembros();                // Muestra un mensaje de error de apertura
void informarErrorIDNoValido();


//      LISTAS MOMENTO        (FUNCIONES QUE USAN NODOS)
void purgarArchivoMiembros();                               // Elimina los miembros no activos del registro y reordena el archivo
nodoMiembro *inicListMiembro();                             // Retorna NULL para los nodoMiembro
nodoMiembro *crearNodoMiembro(stMiembro datosMiembro);      // Crea un nodo y lo retorna cargado
nodoMiembro *agregarAlistaMiembro(nodoMiembro *lista, nodoMiembro *nuevoMiembroNodo); // Agrega el nodo a lo último de la lista
void pasarListaMiembrosAArchivo(nodoMiembro *lista);        // Pasa los nodos miembros al archivo de manera ordenada

//
void mostrarListaMiembros(nodoMiembro *lista);              // Muestra recursivamente la lista
