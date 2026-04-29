/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  declaraciones.h
 * Autor:               CyndxTs o.0?!
/*/

#ifndef DECLARACIONES_H
#define DECLARACIONES_H

// Declaracion de constantes
const int med_IO = 10000;               // Medida máxima de entrada y salida.
const int med_ID = 60;                  // Medida maxima de identificador de un elemento 'conjunto'.
const int med_KW = 30;                  // Medida máxima de identificador de 'PalabraClave' única.
const int med_OP = 15;                  // Medida máxima de identificador de 'Operador' único.
const int max_KW = 60;                  // Cantidad maxima de palabras clave predefinidas.
const int max_OP = 30;                  // Cantidad maxima de operadores predefinidos.
const int max_PO = 15;                  // Cantidad máxima de parámetros de 'Funcion' y operandos de 'Asignación'.

// Definicion: 'Keyword'
typedef struct {
    char identificador[med_KW];
} Keyword;

// Definicion: 'Operator'
typedef struct {
    char identificador[med_OP];
    bool esAcotable;
    bool esSegmentador;
} Operator;

// Definicion: 'Parameter'
typedef struct {
    char *palabraClave;
    char *identificador;
} Parameter;

// Definicion: 'Function'
typedef struct {
    int numParametros;
    Parameter *parametros;
} Function;

// Definicion: 'Operand'
typedef struct {
    char *identificador;
} Operand;

// Definicion: 'Assignment'
typedef struct {
    bool esAgrupada;
    int numOperandos;
    Operand *operandos;
} Assignment;

// Definicion: 'Declaration'
typedef struct {
    char *palabraClave;
    char *identificador;
    char tipo;
    Assignment *asignacion;
    Function *funcion;
} Declaration;

// Definicion: 'Node'
typedef struct node {
    Declaration *declaracion;
    struct node *proximo;
} Node;

// Definicion: 'LDX'
typedef struct {
    Node *inicial;
    Node *final;
} LDX;

// Definicion: 'FormatoDeProcesamiento'
typedef struct {
    bool ajustarPorMargen;              // [#] Cuando se activa, las declaraciones no sobrepasarán el margen de página designado.
    int limitePorMargen;                //  > Cantidad de caracteres hasta el margen de pagina.[Usualmente el predeterminado es 80]
    bool ordenarDeclaraciones;          // [#] Cuando se activa, las declaraciones serán ordenadas de acuerdo a los criterios de ordenamiento designados.
    char criteriosDeOrdenamiento[4];    //  > Criterios de ordenamiento designados.[Criterios de Ordenamiento: "{Tipo De Declaracion}{Keyword}{Identificador}"][Tipo de Ordenamiento: {'A'scendente}{'C'onsecuente}{'D'escendente}]
    bool espaciarSubelementos;          // [#] Cuando se activa, se agregará un espacio tras cada separador de un subelemento con excepción del último.
    bool procesarAsignaciones;          // [#] Cuando se activa, el programa procesará las asignaciones. [Variables globales]
    bool procesarFunciones;             // [#] Cuando se activa, el programa procesará las funciones.
    bool suprimirVariables;             //  > Cuando se activa, no se mostrarán los identificadores de las variables de los parametros de las funciones en definicion.
    char simboloDelimitador;            //  > Simbolo delimitador en la 'Impresion' de funciones.
} ProcessingFormat;

// Declaracion de conjuntos predefinidos
extern const char *agrupadores[];
extern const char espaciadores[];
extern const char modificadores[];
extern const char separadores[];
extern Keyword keywords[max_KW];
extern Operator operators[max_OP];
extern LDX ldx;
extern ProcessingFormat pf;

#endif /* DECLARACIONES_H */
