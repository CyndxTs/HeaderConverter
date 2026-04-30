
/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  converter.h
 * Autor:               CyndxTs
/*/

#ifndef HEADERCONVERTER_CONVERTER_H
#define HEADERCONVERTER_CONVERTER_H
#include <fstream>
using namespace std;
#include "declarations.h"

void actualizarArchivoDePalabrasClave(Keyword *);

void cargarListaDePalabrasClave();

void actualizarArchivoDeOperadores(Operator *);

void cargarListaDeOperadores();

void actualizarArchivoDeFormatoDeProcesamiento(ProcessingFormat);

void cargarFormatoDeProcesamiento();

void headerConversion();

void almacenarProximaPalabraClave(ifstream &, char *);

void procesarProximosIdentificadores(ifstream &, char *, char *, char &);

void procesarSubElementosDeDeclaracion(ifstream &, Declaration &);

void almacenarParametrosDeFuncion(ifstream &, Function &);

void almacenarOperandosDeAsignacion(ifstream &, Assignment &);

void procesarProximosIdentificadores(ifstream &, const Parameter &);

void procesarProximosIdentificadores(ifstream &, const Operand &);

ifstream abrirArchivo_IFS(const char *);

ofstream abrirArchivo_OFS(const char *);

bool esLetra(char);

bool esNumero(char);

bool esPalabraClave(char *);

bool esElemento(char, const char *);

bool esElemento(char, int, const char **);

bool hayDescarteEspecial(ifstream &, char);

bool hayDescarteDeComentario(ifstream &, char);

char obtenerAgrupadorInverso(char);

char *obtenerDinamicoExacto(const char *);

void concatenarPorTipoDeElemento(char *, char *, char);

void descartarProximosDatosDeDeclaracion(ifstream &, char);

void descartarHastaDelimitador(ifstream &, char);

void almacenarHastaDelimitador(ifstream &, char *, char, int &);

void almacenarProximosModificadores(ifstream &, char *);

void espaciarOperadorEnCadena(const char *, char *, bool);

void darWarning(char, const char *);

void limpiarListaDeDeclaraciones();

void imprimirListaDeDeclaraciones(ofstream &);

void imprimirFuncion(ofstream &, Function, int);

void imprimirAsignacion(ofstream &, Assignment, int);

bool seAjustaPorMargen(ofstream &, int, int, int &);

void insertarDeclaracionEnLista(const Declaration &);

bool seInsertaAntesDeNodo(const Declaration &, const Declaration &);

#endif //HEADERCONVERTER_CONVERTER_H