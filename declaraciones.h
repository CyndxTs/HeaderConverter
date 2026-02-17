
/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  declaraciones.h
 * Autor:               CyndxTs o.0?!
/*/

#ifndef DECLARACIONES_H
#define DECLARACIONES_H

// Declaracion de Constantes
const int max_OP    = 50;   // Número máximo de Operandos por 'Asignacion'.
const int max_PM    = 25;   // Número máximo de Parametros en 'Funcion'.
const int med_ID    = 100;  // Medida Maxima de Identificador de un elemento en general.
const int med_KW    = 50;   // Medida Maxima de Identificador de 'KeyWord'.
const int med_OP    = 10;   // Medida Maxima de Identificador de 'Operador'.
const int num_KW    = 50;   // Cantidad Maxima de Palabras Clave Predefinidas.
const int num_OP    = 30;   // Cantidad Maxima de Operadores Predefinidos.
const int tamLinea  = 60;   // Cantidad Maxima de Caracteres por Linea de Consola

// Definicion: 'KeyWord' [Palabra Clave]
typedef struct{
    char identificador[med_KW];
} KeyWord;
// Definicion: 'Operador'
typedef struct{
    char identificador[med_OP];
    bool esAcotable;
    bool esSegmentador;
} Operador;
// Definicion: 'Parametro'
typedef struct{
    char keyWord[med_KW];
    char identificador[med_ID];
    int posConjunta;
} Parametro;
// Definicion: 'Operando'
typedef struct{
    char identificador[med_ID];
    int posConjunta;
} Operando;
// Definicion: 'Asignacion'
typedef struct{
    int posApertura;
    int numOperandos;
    Operando operandos[max_OP];
    bool esAgrupada;
} Asignacion;
// Definicion: 'Funcion'
typedef struct{
    int posApertura;
    int numParametros;
    Parametro parametros[max_PM];
} Funcion;
// Definicion: 'Declaracion'
typedef struct{
    char keyWord[med_KW];
    char identificador[med_ID];
    char tipo;
    Asignacion asignacion;
    Funcion funcion;
} Declaracion;
// Definicion: 'Nodo'
typedef struct nodo{
    Declaracion declaracion;
    struct nodo *proximo;
} Nodo;
// Declaracion: 'LDX'
typedef struct{
    Nodo *inicial;
    Nodo *final;
} LDX;

#endif /* DECLARACIONES_H */
