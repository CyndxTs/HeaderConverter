
/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  converter.h
 * Autor:               CyndxTs o.0?!
/*/

#ifndef CONVERTER_H
#define CONVERTER_H
#include "declaraciones.h"

                        /* Controladores Del Programa */

// Controladores Generales
const bool ajustarPorMargen = true;                                             // [#] Cuando se activa, las declaraciones no sobrepasarán el margen de página designado.
    const int limitePorMargen = 80;                                             // [|] Cantidad de caracteres hasta el margen de pagina.[Usualmente el predeterminado es 80]
const bool ordenarDeclaraciones = true;                                         // [#] Cuando se activa, las declaraciones serán ordenadas de acuerdo a los criterios de ordenamiento designados.
    const char criteriosDeOrdenamiento[4] = "ACC";                              // [|] Criterios de ordenamiento designados.[Criterios de Ordenamiento: "{Tipo De Declaracion}{KeyWord}{Identificador}"][Tipo de Ordenamiento: {'A'scendente}{'C'onsecuente}{'D'escendente}]
// Controladores de Asignaciones [Variables Globales]
const bool procesarAsignaciones = true;                                         // [#] Cuando se activa, el programa procesará las asignaciones. [Variables globales]
// Controladores de Funciones
const bool procesarFunciones = true;                                            // [#] Cuando se activa, el programa procesará las funciones.
    const bool suprimirVariables = true;                                        // [|] Cuando se activa, no se mostrarán los identificadores de las variables de los parametros de las funciones en definicion.
    const char simboloDelimitador = ';';                                        // [|] Simbolo delimitador en la 'Impresion' de funciones. Por ejemplo, puede cambiarlo a '{' en caso quiera ajustar las cabezeras del cpp.)

                        /* Declaracion de Constantes */

// Definición de Diccionario de KeyWords.[Puede agregar una nueva en caso genere un TAD nuevo.{Formato: '{"<Nombre de KeyWord>"}'}]
const KeyWord keyWords[num_KW] = {{"void"},{"char"},{"double"},{"int"},
                                  {"bool"},{"const"},{"struct"},{"operator"},
                                  {"private"},{"public"},{"ifstream"},
                                  {"ofstream"},{"fstream"},{"ostream"},
                                  {"istream"},{"LDX"},{"Nodo"},{"Declaracion"},
                                  {"Asignacion"},{"Funcion"},{"Parametro"},
                                  {"Operando"},{"KeyWord"},{"Operador"},
                                  {"SubElemento"},{"Cliente"},{"Producto"},
                                  {"Pedido"},{"Curso"},{"Alumno"},{"Libro"},
                                  {0}};

// Definición de Diccionario de Operadores.[Puede agregar uno nuevo en caso lo considere necesario.{Formato: '{"<Simbolo de Operador>",<[T/F]:¿Se le puede acotar por letras?>,<[T/F]:¿Debe segmentar operandos?>}'}]
const Operador operators[num_OP] = {{"+",true,true},{"-",true,true},
                                    {"*",true,false},{"/",true,false},
                                    {":",true,false},{">",true,true},
                                    {"<",true,true},{">=",true,true},
                                    {"<=",true,true},{"==",true,true},
                                    {"!=",true,true},{"!",true,false},
                                    {"||",true,true},{"&&",true,true},
                                    {"or",false,true},{"and",false,true},
                                    {"xor",false,true},{0}};

// Definicion de Conjuntos Predefinidos.[Se recomienda, no editarlos..]
const char agrupadores[10][3] = {{'"','"',0},{39,39,0},{'(',')',0},{'[',']',0},
                                 {'{','}',0},{0}};
const char espaciadores[10] = {'\t','\n',' ',0};
const char modificadores[10] = {'&','*',0};
const char separadores[10] = {'\n',' ',',',';',0};

                         /* Declaracion de Funciones */

void HeaderConversion(const char *,const char *);

void procesarDeclaracionPorTipo(ifstream &,LDX &,Declaracion &);

void imprimirLista(ofstream &,LDX);

void descartarProximosDatosDeDeclaracion(ifstream &,char);

void insertarOrdenado(LDX &,Declaracion);

void imprimirFuncion(ofstream &,Funcion,bool &);

void imprimirAsignacion(ofstream &,Asignacion,bool &);

bool seInsertaAntesDeNodo(Declaracion,Declaracion);

bool seAjustaPorMargen(ofstream &,int,int,int &);

ifstream abrirArchivo_IFS(const char *);

ofstream abrirArchivo_OFS(const char *);

bool esElemento(char,const char *);

bool esAgrupador(char,int);

bool esLetra(char);

bool esKeyWord(char *);

bool hayDescarteEspecial(ifstream &,char);

bool hayDescarteDeComentario(ifstream &,char);

char obtenerAgrupadorInverso(char);

void espaciarOperadorEnCadena(const char *,char *,bool);

void almacenarHastaDelimitador(ifstream &,char *,char,int &);

void descartarHastaDelimitador(ifstream &,char);

void almacenarProximaKeyWord(ifstream &,char *);

void concatenarNuevoElemento(char *,char *,char);

void agregarProximosModificadores(ifstream &,char *);

void procesarProximosIdentificadores(ifstream &,Declaracion &);

void procesamientoPorTipoDeLetra(ifstream &,Declaracion &,char *,char,bool,
                                 int &,bool &,bool &);

void obtenerParametros(ifstream &,Funcion &);

void procesarProximosIdentificadores(ifstream &,Parametro &);

void procesamientoPorTipoDeLetra(ifstream &,Parametro &,char *,char,bool &,
                                 int &,bool &,bool &,bool &);

void procesarSubOperandos(ifstream &,char *);

void obtenerOperandos(ifstream &,Asignacion &);

void procesarProximosIdentificadores(ifstream &,Operando &);

void darWarning(char,const char * = "");

#endif /* CONVERTER_H */
