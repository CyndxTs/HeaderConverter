
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

// Funciones principales
void actualizarArchivoDePalabrasClave(Keyword *palabrasClave);
void cargarListaDePalabrasClave();
void actualizarArchivoDeOperadores(Operator *operadores);
void cargarListaDeOperadores();
void actualizarArchivoDeFormatoDeProcesamiento(ProcessingFormat formatoDeProcesamiento);
void cargarFormatoDeProcesamiento();
void headerConversion();

// Funciones secundarias
void almacenarProximaPalabraClave(ifstream &archOrigen, char *palabraClave);
void procesarProximosIdentificadores(ifstream &archOrigen, char *palabraClave, char *identificador, char &tipoDeclaracion);
void procesarSubElementosDeDeclaracion(ifstream &archOrigen, Declaration &declaracion);

// Funciones derivadas
void almacenarParametrosDeFuncion(ifstream &archOrigen, Function &funcion);
void almacenarOperandosDeAsignacion(ifstream &archOrigen, Assignment &asignacion);

// Funciones subderivadas
void procesarProximosIdentificadores(ifstream &archOrigen, const Parameter &parametro);
void procesarProximosIdentificadores(ifstream &archOrigen, const Operand &operando);

// Funciones auxiliares
ifstream abrirArchivo_IFS(const char *nombArch);
ofstream abrirArchivo_OFS(const char *nombArch);
bool esLetra(char simbolo);
bool esNumero(char simbolo);
bool esPalabraClave(char *cadena);
bool esElemento(char simbolo, const char *conjunto);
bool esElemento(char simbolo, int j, const char **conjunto);
bool hayDescarteEspecial(ifstream &archOrigen, char letra);
bool hayDescarteDeComentario(ifstream &archOrigen, char letra);
char obtenerAgrupadorInverso(char simbolo);
char *obtenerDinamicoExacto(const char *cadena);
void concatenarPorTipoDeElemento(char *cadDestino, char *cadOrigen, char tipo);
void descartarProximosDatosDeDeclaracion(ifstream &archOrigen, char tipo);
void descartarHastaDelimitador(ifstream &archOrigen, char delimitador);
void almacenarHastaDelimitador(ifstream &archOrigen, char *cadena, char delimitador, int &posCad);
void almacenarProximosModificadores(ifstream &archOrigen, char *cadena);
void espaciarOperadorEnCadena(const char *op, char *cad, bool espaciarAlFinal);
void darWarning(char warningID, const char *reason);

// Funciones de lista
void limpiarListaDeDeclaraciones();
void imprimirListaDeDeclaraciones(ofstream &archSalida);
void imprimirFuncion(ofstream &archSalida, Function funcion, int posApertura);
void imprimirAsignacion(ofstream &archSalida, Assignment asignacion, int posApertura);
bool seAjustaPorMargen(ofstream &archSalida, int posApertura, int posConjunta, int &posColumna);
void insertarDeclaracionEnLista(const Declaration &declaracion);
bool seInsertaAntesDeNodo(const Declaration &declaracion, const Declaration &d_Aux);

#endif //HEADERCONVERTER_CONVERTER_H