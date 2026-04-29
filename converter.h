
/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  converter.h
 * Autor:               CyndxTs o.0?!
/*/

#ifndef CONVERTER_H
#define CONVERTER_H
#include "declaraciones.h"

void HeaderConversion(ifstream &archOrigen,
                      ofstream &archDestino);

void almacenarProximaPalabraClave(ifstream &archOrigen,
                                  char *palabraClave);

void procesarProximosIdentificadores(ifstream &archOrigen,
                                     char *palabraClave,
                                     char *identificador,
                                     char &tipoDeclaracion);

void procesarSubElementosDeDeclaracion(ifstream &archOrigen,
                                       Declaration &declaracion);

                     /* - / Funciones Secundarias / - */

void almacenarParametrosDeFuncion(ifstream &archOrigen,
                                  Function &funcion);

void almacenarOperandosDeAsignacion(ifstream &archOrigen,
                                    Assignment &asignacion);

                     /* - / Funciones Derivadas / - */

void procesarProximosIdentificadores(ifstream &archOrigen,
                                     Parameter &parametro);

void procesarProximosIdentificadores(ifstream &archOrigen,
                                     Operand &operando);

                     /* - / Funciones Auxiliares / - */

ifstream abrirArchivo_IFS(const char *nombArch);

ofstream abrirArchivo_OFS(const char *nombArch);

bool esLetra(char simbolo);

bool esNumero(char simbolo);

bool esPalabraClave(char *cadena);

bool esElemento(char simbolo,
                const char *conjunto);

bool esElemento(char simbolo,
                int j,
                const char **conjunto);

bool hayDescarteEspecial(ifstream &archOrigen,
                         char letra);

bool hayDescarteDeComentario(ifstream &archOrigen,
                             char letra);

char obtenerAgrupadorInverso(char simbolo);

char *obtenerDinamicoExacto(const char *cadena);

void concatenarPorTipoDeElemento(char *cadDestino,
                                 char *cadOrigen,
                                 char tipo);

void descartarProximosDatosDeDeclaracion(ifstream &archOrigen,
                                         char tipo);

void descartarHastaDelimitador(ifstream &archOrigen,
                               char delimitador);

void almacenarHastaDelimitador(ifstream &archOrigen,
                               char *cadena,
                               char delimitador,
                               int &posCad);

void almacenarProximosModificadores(ifstream &archOrigen,
                                    char *cadena);

void espaciarOperadorEnCadena(const char *op,
                              char *cad,
                              bool espaciarAlFinal);

void darWarning(char warningID,
                const char *reason);

void limpiarListaDeDeclaraciones();

void imprimirListaDeDeclaraciones(ofstream &archSalida);

void imprimirFuncion(ofstream &archSalida,
                     Function funcion, int);

void imprimirAsignacion(ofstream &archSalida,
                        Assignment asignacion, int);

bool seAjustaPorMargen(ofstream &archSalida,
                       int posApertura,
                       int posConjunta,
                       int &posColumna);

void insertarDeclaracionEnLista(Declaration declaracion);

bool seInsertaAntesDeNodo(Declaration declaracion,
                          Declaration d_Aux);

#endif /* CONVERTER_H */
