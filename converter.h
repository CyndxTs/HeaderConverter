
/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  converter.h
 * Autor:               CyndxTs
/*/

#ifndef HEADERCONVERTER_CONVERTER_H
#define HEADERCONVERTER_CONVERTER_H
#include "declarations.h"
#include <fstream>
using namespace std;

bool esElemento(char , const char *);

bool esElemento(char , int , const char **);

bool esLetra(char);

bool esNumero(char);

bool esPalabraClave(char *);

bool existeKW(const char *, Keyword *, int);

bool hayDescarteDeComentario(ifstream &, char);

bool hayDescarteEspecial(ifstream &, char);

bool seAjustaPorMargen(ofstream &, int , int , int &);

bool seInsertaAntesDeNodo(Declaration , Declaration);

char obtenerAgrupadorInverso(char);

char *obtenerDinamicoExacto(const char *);

ifstream abrirArchivo_IFS(const char *);

int buscarOperador(const char *, Operator *, int);

ofstream abrirArchivo_OFS(const char *);

void HeaderConversion(ifstream &, ofstream &);

void actualizarFormatoDeProcesamiento(ProcessingFormat);

void actualizarListaDeOperadores(Operator *);

void actualizarListaDePalabrasClave(Keyword *);

void almacenarHastaDelimitador(ifstream &, char *, char , int &);

void almacenarOperandosDeAsignacion(ifstream &, Assignment &);

void almacenarParametrosDeFuncion(ifstream &, Function &);

void almacenarProximaPalabraClave(ifstream &, char *);

void almacenarProximosModificadores(ifstream &, char *);

void cargarFormatoDeProcesamiento();

void cargarListaDeOperadores();

void cargarListaDePalabrasClave();

void concatenarPorTipoDeElemento(char *, char *, char);

void darWarning(char , const char *);

void descartarHastaDelimitador(ifstream &, char);

void descartarProximosDatosDeDeclaracion(ifstream &, char);

void espaciarOperadorEnCadena(const char *, char *, bool);

void imprimirAsignacion(ofstream &, Assignment , int);

void imprimirFuncion(ofstream &, Function , int);

void imprimirListaDeDeclaraciones(ofstream &);

void insertarDeclaracionEnLista(Declaration);

void limpiarListaDeDeclaraciones();

void procesarProximosIdentificadores(ifstream &, char *, char *, char &);

void procesarProximosIdentificadores(ifstream &, Parameter &);

void procesarProximosIdentificadores(ifstream &, Operand &);

void procesarSubElementosDeDeclaracion(ifstream &, Declaration &);

#endif //HEADERCONVERTER_CONVERTER_H