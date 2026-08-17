# Library Project — Gestor de Biblioteca

Sistema de consola en C para gestionar una biblioteca: alta y baja de socios, catálogo de libros y préstamos con control de vencimientos y recargos por atraso. Es el trabajo final que hice de forma individual en la facultad, así que lo pensé en base a estructuras de datos vistas en la carrera en vez de usar una base de datos externa.

## ¿Qué hace?

- **Gestión de miembros**: alta, baja (baja lógica, no se borra el historial), modificación y búsqueda de socios por distintos campos.
- **Gestión de libros**: alta, baja, modificación y búsqueda por título, autor, género o ISBN, con control de disponibilidad.
- **Préstamos**: registra el préstamo de un libro a un socio con fecha de vencimiento (7 días), calcula recargos por atraso ($150 por día) y lleva la deuda acumulada de cada socio.
- **Persistencia**: todos los datos se guardan en archivos binarios (`.bin`), así que la información no se pierde al cerrar el programa.

## Estructuras de datos usadas

Es la parte que más practiqué con este proyecto:

- **Árbol binario de búsqueda**: los libros se organizan en un árbol por categoría, para poder recorrerlos y buscarlos de forma ordenada.
- **Listas enlazadas**: se usan para manejar las categorías de libros y el historial de préstamos de cada socio.
- **Structs anidados**: `stLibro`, `stMiembro`, `stPrestamo` y `stFecha` para modelar cada entidad del sistema.
- **Manejo de archivos binarios**: lectura y escritura de estructuras completas a disco con `fread`/`fwrite`.

## Tecnologías

- **C** (Code::Blocks como IDE / build system)
- Sin librerías externas más allá de la librería estándar (`stdio`, `stdlib`, `stdbool`, `time`, `windows.h` para el color de la consola)

## Estructura del proyecto

```
LibraryProyect/
├── main.c              # Punto de entrada, menú principal
├── dependenciasLib.c/h # Funciones auxiliares (impresión, validación, colores de consola)
├── librosLib.c/h       # Lógica de libros y árbol binario
├── miembrosLib.c/h     # Lógica de socios
├── prestamosLib.c/h    # Lógica de préstamos, vencimientos y recargos
└── datos/              # Archivos binarios donde persiste la información
```

## Cómo correrlo

1. Abrí `tpFinal.cbp` con Code::Blocks (o compilá los `.c` con GCC/MinGW si preferís línea de comandos).
2. Compilá y ejecutá — el programa crea automáticamente la carpeta `datos/` con los archivos binarios la primera vez que corre.
3. Es un programa de consola: se maneja completamente por menús numerados.

## Documentación adicional

En la carpeta `Documentacion/` está el detalle de todas las funciones de cada módulo (`Documentacion.html`) y los diagramas de las estructuras de libros, miembros y préstamos, hechos antes de empezar a programar.

## Limitaciones conocidas / próximas mejoras

- La persistencia es en archivos binarios locales; en una versión más avanzada usaría una base de datos relacional.
- Las rutas de archivo (`windows.h`, `mkdir`) están pensadas para Windows y no son portables a Linux/Mac tal cual están.
- Se podría separar mejor la lógica de negocio de las funciones de impresión por consola.
