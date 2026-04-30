
/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  converter.cpp
 * Autor:               CyndxTs
/*/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#include "converter.h"

                      /* - / Funciones Principales / - */

// Modulo de actualizacion de archivo de palabras clave
void actualizarArchivoDePalabrasClave(Keyword *palabrasClave) {
    // Declaracion & Inicializacion de variables
    bool kwRepetido;
    int cantNuevos = 0, cantFinales = 0;
    Keyword palabrasClaveFinales[max_KW]{};
    // Medicion de lista nueva
    while (palabrasClave[cantNuevos].identificador[0]) cantNuevos++;
    // Exclusion de repetidos
    for (int i = 0; i < cantNuevos && cantFinales < max_KW; i++) {
        kwRepetido = false;
        for (int j = 0; j < cantFinales; j++) {
            if (strcmp(palabrasClave[i].identificador, palabrasClaveFinales[j].identificador) == 0) {
                kwRepetido = true;
                break;
            }
        }
        if(!kwRepetido) {
            strcpy(palabrasClaveFinales[cantFinales].identificador, palabrasClave[i].identificador);
            cantFinales++;
        }
    }
    // Apertura de archivo de salida
    ofstream archSalida = abrirArchivo_OFS("../resources/Keywords.csv");
    // Actualizacion de archivo de palabras clave
    for (int i = 0; i < cantFinales; i++) archSalida<<palabrasClaveFinales[i].identificador<<endl;
    // Cierre de archivo de salida
    archSalida.close();
}
// Modulo de carga de lista de palabras clave
void cargarListaDePalabrasClave() {
    // Apertura de archivo de entrada
    ifstream archEntrada = abrirArchivo_IFS("../resources/Keywords.csv");
    // Inicializacion de variables
    int cantKw = 0;
    char cadAux[med_KW]{};
    // Carga de lista de palabras clave
    while (1) {
        archEntrada>>cadAux;
        if (archEntrada.eof()) break;
        strcpy(keywords[cantKw].identificador, cadAux);
        cantKw++;
    }
    // Validacion de delimitacion de lista
    if (cantKw < max_KW) keywords[cantKw].identificador[0] = 0;
    // Cierre de archivo de entrada
    archEntrada.close();
}
// Modulo de actualizacion de archivo de operadores
void actualizarArchivoDeOperadores(Operator *operadores) {
    // Declaracion & Inicializacion de variables
    int cantArchivados = 0;
    char cadAux[med_OP]{};
    Operator operadoresArchivados[max_OP]{};
    // Apertura de archivo de entrada
    ifstream archEntrada = abrirArchivo_IFS("../resources/Operators.csv");
    // Lectura de operadores archivados
    while (1) {
        archEntrada.getline(cadAux, med_OP, ',');
        if(archEntrada.eof()) break;
        strcpy(operadoresArchivados[cantArchivados].identificador, cadAux);
        archEntrada.getline(cadAux, med_OP, ',');
        operadoresArchivados[cantArchivados].esAcotable = (strcmp(cadAux, "true") == 0);
        archEntrada.getline(cadAux, med_OP);
        operadoresArchivados[cantArchivados].esSegmentador = (strcmp(cadAux, "true") == 0);
        cantArchivados++;
    }
    // Cierre de archivo de entrada
    archEntrada.close();
    // Actualizacion de operadores
    for (int i = 0; i < cantArchivados; i++) {
        for (int j = 0; operadores[j].identificador[0]; j++) {
            if (strcmp(operadoresArchivados[i].identificador, operadores[j].identificador) == 0) {
                operadoresArchivados[i].esAcotable = operadores[j].esAcotable;
                operadoresArchivados[i].esSegmentador = operadores[j].esSegmentador;
            }
        }
    }
    // Apertura de archivo de salida
    ofstream archSalida = abrirArchivo_OFS("../resources/Operators.csv");
    // Actualizacion de archivo de operadores
    for (int i = 0; i < cantArchivados; i++) {
        archSalida<<operadoresArchivados[i].identificador<<",";
        archSalida<<(operadoresArchivados[i].esAcotable ? "true" : "false")<<",";
        archSalida<<(operadoresArchivados[i].esSegmentador ? "true" : "false")<<endl;
    }
    // Cierre de archivo de salida
    archSalida.close();
}
// Modulo de carga de lista de operadores
void cargarListaDeOperadores() {
    // Apertura de archivo de entrada
    ifstream archEntrada = abrirArchivo_IFS("../resources/Operators.csv");
    // Inicializacion de variables
    int posOp = 0;
    char cadAux[med_OP]{};
    // Carga de lista de operadores
    while (1) {
        archEntrada.getline(cadAux, med_OP, ',');
        if (archEntrada.eof()) break;
        strcpy(operators[posOp].identificador, cadAux);
        archEntrada.getline(cadAux, med_OP, ',');
        operators[posOp].esAcotable = (strcmp(cadAux, "true") == 0);
        archEntrada>>cadAux;
        operators[posOp].esSegmentador = (strcmp(cadAux, "true") == 0);
        archEntrada>>ws;
        posOp++;
    }
    // Cierre de archivo de entrada
    archEntrada.close();
}
// Modulo de actualizacion de archivo de formato de procesamiento
void actualizarArchivoDeFormatoDeProcesamiento(ProcessingFormat formatoDeProcesamiento) {
    // Apertura de archivo de salida
    ofstream archSalida = abrirArchivo_OFS("../resources/ProcessingFormat.csv");
    // Actualizacion de formato de procesamiento
    archSalida<<"adjustMargin,"<<(formatoDeProcesamiento.ajustarPorMargen ? "true" : "false")<<endl;
    archSalida<<"marginLimit,"<<formatoDeProcesamiento.limitePorMargen<<endl;
    archSalida<<"sortDeclarations,"<<(formatoDeProcesamiento.ordenarDeclaraciones ? "true" : "false")<<endl;
    archSalida<<"sortingCriteria,"<<formatoDeProcesamiento.criteriosDeOrdenamiento<<endl;
    archSalida<<"spaceSubelements,"<<(formatoDeProcesamiento.espaciarSubelementos ? "true" : "false")<<endl;
    archSalida<<"processAssignments,"<<(formatoDeProcesamiento.procesarAsignaciones ? "true" : "false")<<endl;
    archSalida<<"processFunctions,"<<(formatoDeProcesamiento.procesarFunciones ? "true" : "false")<<endl;
    archSalida<<"suppressVariables,"<<(formatoDeProcesamiento.suprimirVariables ? "true" : "false")<<endl;
    archSalida<<"delimiterSymbol,"<<formatoDeProcesamiento.simboloDelimitador<<endl;
    // Cierre de archivo de salida
    archSalida.close();
}
// Modulo de carga de formato de procesamiento
void cargarFormatoDeProcesamiento() {
    // Apertura de archivo de entrada
    ifstream archEntrada = abrirArchivo_IFS("../resources/ProcessingFormat.csv");
    // Inicializacion de variables
    char cadAtributo[med_ID]{}, cadValor[med_OP]{};
    pf.ajustarPorMargen = true;
    pf.limitePorMargen = 80;
    pf.ordenarDeclaraciones = true;
    strcpy(pf.criteriosDeOrdenamiento, "AAA");
    pf.espaciarSubelementos = true;
    pf.procesarAsignaciones = true;
    pf.procesarFunciones = true;
    pf.suprimirVariables = false;
    pf.simboloDelimitador = ';';
    // Carga de formato de procesamiento
    while (1) {
        archEntrada.getline(cadAtributo, med_ID, ',');
        if(archEntrada.eof()) break;
        archEntrada.getline(cadValor, med_OP);
        if (strcmp(cadAtributo, "adjustMargin") == 0) {
            pf.ajustarPorMargen = (strcmp(cadValor, "true") == 0);
        } else if (strcmp(cadAtributo, "marginLimit") == 0) {
            pf.limitePorMargen = atoi(cadValor);
        } else if (strcmp(cadAtributo, "sortDeclarations") == 0) {
            pf.ordenarDeclaraciones = (strcmp(cadValor, "true") == 0);
        } else if (strcmp(cadAtributo, "sortingCriteria") == 0) {
            strcpy(pf.criteriosDeOrdenamiento, cadValor);
        } else if (strcmp(cadAtributo, "spaceSubelements") == 0) {
            pf.espaciarSubelementos = (strcmp(cadValor, "true") == 0);
        } else if (strcmp(cadAtributo, "processAssignments") == 0) {
            pf.procesarAsignaciones = (strcmp(cadValor, "true") == 0);
        } else if (strcmp(cadAtributo, "processFunctions") == 0) {
            pf.procesarFunciones = (strcmp(cadValor, "true") == 0);
        } else if (strcmp(cadAtributo, "suppressVariables") == 0) {
            pf.suprimirVariables = (strcmp(cadValor, "true") == 0);
        } else if (strcmp(cadAtributo, "delimiterSymbol") == 0) {
            pf.simboloDelimitador = cadValor[0];
        }
    }
    // Cierre de archivo de entrada
    archEntrada.close();
}
// Modulo de conversion de archivo
void headerConversion() {
    // Apertura de archivos de entrada y salida
    ifstream archOrigen = abrirArchivo_IFS("../resources/Source.txt");
    ofstream archDestino = abrirArchivo_OFS("../resources/Conversion.txt");
    // Proceso de conversion de archivo
    while(1){
        // Inicializacion de variables base para declaracion
        char palabraClave[med_KW]{}, identificador[med_ID]{}, tipo = 0;
        // Busqueda y almacenamiento de proxima palabra clave
        almacenarProximaPalabraClave(archOrigen, palabraClave);
        // Validacion de fin de archivo
        if(archOrigen.eof()) break;
        // Procesamiento de proximos identificadores para declaracion
        procesarProximosIdentificadores(archOrigen, palabraClave, identificador, tipo);
        // Inicializacion de nueva declaracion
        Declaration declaracion {obtenerDinamicoExacto(palabraClave), obtenerDinamicoExacto(identificador), tipo};
        // Procesamiento de subelementos de declaracion
        procesarSubElementosDeDeclaracion(archOrigen, declaracion);
    }
    // Impresion de lista de declaraciones
    imprimirListaDeDeclaraciones(archDestino);
    // Limpieza de lista de declaraciones
    limpiarListaDeDeclaraciones();
    // Cierre de archivos de entrada y salida
    archOrigen.close();
    archDestino.close();
}

                      /* - / Funciones Secundarias / - */

// Modulo de busqueda y almacenamiento de proxima palabra clave
void almacenarProximaPalabraClave(ifstream &archOrigen, char *palabraClave){
    // Declaracion de variables
    bool KwCoincidentes[med_KW]{};
    int posExacta = -1;
    // Proceso de busqueda y almacenamiento
    for (int i = 0; 1; i++){
        bool haySimilares = false;
        char letra = archOrigen.get();
        // Validacion de fin de archivo
        if (archOrigen.eof()) return;
        // Validacion de descarte especial
        if(not hayDescarteEspecial(archOrigen,letra)){
            // Busqueda de cadena clave por posicion de letra
            for(int posKw = 0; keywords[posKw].identificador[0]; posKw++){
                if(KwCoincidentes[posKw] or i == 0){
                    if(keywords[posKw].identificador[i] == letra){
                        if(keywords[posKw].identificador[i+1] == 0){
                            strcpy(palabraClave, keywords[posKw].identificador);
                            posExacta = posKw;
                        }
                        KwCoincidentes[posKw] = true;
                        haySimilares = true;
                    } else KwCoincidentes[posKw] = false;
                }
            }
        }
        // Validacion de existencia de coincidencias
        if (not haySimilares){
            // Validacion de existencia de solucion exacta encontrada
            if(posExacta != -1){
                if(esElemento(letra, modificadores)) archOrigen.unget();
                return;
            }
            i = -1;
        }
    }
}
// Modulo de procesamiento de proximos identificadores de elementos de declaracion
void procesarProximosIdentificadores(ifstream &archOrigen, char *palabraClave, char *identificador, char &tipoDeclaracion){
    // Declaration de variables
    bool particion = false, sobrecargaDeOp = false;
    char cadAux[med_ID]{};
    // Procesamiento de identificadores
    for(int posCad = 0; 1; posCad++){
        char letra = archOrigen.get();
        cadAux[posCad] = 0;
        // Validacion de descarte especial
        if(hayDescarteEspecial(archOrigen, letra)){
            if(posCad > 0){
                if(esPalabraClave(cadAux)){
                    concatenarPorTipoDeElemento(palabraClave, cadAux, 'K');
                    if(strcmp(cadAux, "operator") == 0) sobrecargaDeOp = true;
                    posCad = 0;
                } else particion = true;
            }
            posCad--;
        } else {
            if(letra == '('){
                strcpy(identificador, cadAux);
                strcat(identificador, "(");
                tipoDeclaracion = 'F';
                break;
            } else if(letra == ';'){
                strcpy(identificador, cadAux);
                strcat(identificador, ";");
                tipoDeclaracion = 'A';
                break;
            } else if(not sobrecargaDeOp and letra == '='){
                strcpy(identificador, cadAux);
                strcat(identificador, " =");
                tipoDeclaracion = 'A';
                break;
            } else if(not sobrecargaDeOp and esElemento(letra, modificadores)){
                if(esPalabraClave(cadAux)) {
                    concatenarPorTipoDeElemento(palabraClave, cadAux, 'K');
                } else if(posCad > 0) darWarning('P',"'Declaration'");
                archOrigen.unget();
                almacenarProximosModificadores(archOrigen, palabraClave);
                posCad = -1;
            } else if(esElemento(letra, 0, agrupadores)){
                cadAux[posCad++] = letra;
                letra = obtenerAgrupadorInverso(letra);
                almacenarHastaDelimitador(archOrigen, cadAux, letra, posCad);
                posCad--;
            } else{
                if(particion) darWarning('P',"'Declaration'");
                else cadAux[posCad] = letra;
            }
        }
    }
}
// Modulo de procesmiento de subelementos de declaracion
void procesarSubElementosDeDeclaracion(ifstream &archOrigen, Declaration &declaracion){
    // Validacion por tipo de declaracion
    if(declaracion.tipo == 'F'){
        // Validacion de procesamiento de funciones
        if(pf.procesarFunciones){
            Function *pFuncion = new Function;
            pFuncion->numParametros = 0;
            pFuncion->parametros = new Parameter[max_PO] {};
            almacenarParametrosDeFuncion(archOrigen, *pFuncion);
            declaracion.funcion = pFuncion;
            insertarDeclaracionEnLista(declaracion);
        }
        descartarProximosDatosDeDeclaracion(archOrigen,'F');
    } else {
        // Validacion de procesamiento de asignaciones
        if(pf.procesarAsignaciones){
            Assignment *pAsignacion = new Assignment;
            pAsignacion->esAgrupada = false;
            pAsignacion->numOperandos = 0;
            pAsignacion->operandos = new Operand[max_PO] {};
            almacenarOperandosDeAsignacion(archOrigen, *pAsignacion);
            declaracion.asignacion = pAsignacion;
            insertarDeclaracionEnLista(declaracion);
        } else descartarProximosDatosDeDeclaracion(archOrigen,'A');
    }
}

                      /* - / Funciones Derivadas / - */

// Modulo de almacenamiento de parametros de funcion
void almacenarParametrosDeFuncion(ifstream &archOrigen, Function &funcion){
    // Almacenamiento de parametros de funcion
    for(int pm = 0; 1; pm++){
        // Inicializacion de nuevo parametro
        Parameter parametro {};
        parametro.palabraClave = new char[med_ID] {};
        parametro.identificador = new char[med_ID] {};
        // Procesamiento de identificadores de parametro
        procesarProximosIdentificadores(archOrigen, parametro);
        // Actualizacion de datos de parametro
        funcion.parametros[pm] = parametro;
        funcion.numParametros++;
        // Validacion de fin de funcion
        archOrigen.unget();
        if(archOrigen.get() == ')') break;
        // Validacion de espaciacion de subelementos
        if(pf.espaciarSubelementos) strcat(funcion.parametros[pm].identificador, " ");
    }
}
// Modulo de almacenamiento de operandos de asignacion
void almacenarOperandosDeAsignacion(ifstream &archOrigen, Assignment &asignacion){
    // Validacion de asignacion sin inicializacion
    archOrigen.unget();
    if(archOrigen.get() == ';') return;
    // Validacion de asignacion agrupada
    archOrigen>>ws;
    if(archOrigen.get() == '{') asignacion.esAgrupada = true;
    else archOrigen.unget();
    // Almacenamiento de operandos de asignacion
    for(int op = 0; 1; op++){
        // Inicializacion de nuevo operando
        Operand operando {};
        operando.identificador = new char[med_ID] {};
        // Procesamiento de identificadores de operando
        procesarProximosIdentificadores(archOrigen, operando);
        // Actualizacion de datos de operando
        asignacion.operandos[op] = operando;
        asignacion.numOperandos++;
        // Validacion de fin de operando
        archOrigen.unget();
        char cAnt = archOrigen.get();
        if(cAnt == ';' or cAnt == '}') break;
    }
}

                        /* - / Funciones SubDerivadas / - */

// Modulo de procesamiento de identificadores de parametro
void procesarProximosIdentificadores(ifstream &archOrigen, Parameter &parametro){
    // Declaracion de variables
    bool particion = false;
    char cadAux[med_ID]{};
    // Procesamiento de identificadores
    for(int posCad = 0; 1; posCad++){
        char letra = archOrigen.get();
        cadAux[posCad] = 0;
        // Validacion de descarte especial
        if(hayDescarteEspecial(archOrigen, letra)){
            if(posCad > 0){
                if(esPalabraClave(cadAux)){
                    concatenarPorTipoDeElemento(parametro.palabraClave, cadAux, 'K');
                    posCad = 0;
                } else particion = true;
            }
            posCad--;
        } else{
            if(esElemento(letra, separadores)) {
                if(esPalabraClave(cadAux)){
                    cadAux[posCad++] = letra;
                    cadAux[posCad] = 0;
                    concatenarPorTipoDeElemento(parametro.palabraClave, cadAux, 'K');
                    posCad = 0;
                } else {
                    if(pf.suprimirVariables) posCad = 0;
                    cadAux[posCad++] = letra;
                    cadAux[posCad] = 0;
                    strcat(parametro.identificador, cadAux);
                }
                break;
            } else if(letra == '='){
                descartarHastaDelimitador(archOrigen, ',');
                archOrigen.unget();
                posCad--;
            } else if(esElemento(letra, modificadores)){
                if(esPalabraClave(cadAux)) concatenarPorTipoDeElemento(parametro.palabraClave, cadAux, 'K');
                else if(posCad > 0) darWarning('P',"'Parameter' de 'Funcion'");
                archOrigen.unget();
                almacenarProximosModificadores(archOrigen, parametro.palabraClave);
                posCad = -1;
            } else if(esElemento(letra, 0, agrupadores)){
                if(esPalabraClave(cadAux)){
                    concatenarPorTipoDeElemento(parametro.palabraClave, cadAux,'K');
                    posCad = 0;
                } else if(pf.suprimirVariables) posCad = 0;
                cadAux[posCad++] = letra;
                letra = obtenerAgrupadorInverso(letra);
                almacenarHastaDelimitador(archOrigen, cadAux, letra, posCad);
                cadAux[posCad] = 0;
                strcat(parametro.identificador, cadAux);
                posCad = -1;
                particion = true;
            } else{
                if(particion) darWarning('P',"'Parameter' de 'Funcion'");
                else cadAux[posCad] = letra;
            }
        }
    }
}
// Modulo de procesamiento de identificadores de operando
void procesarProximosIdentificadores(ifstream &archOrigen, Operand &operando){
    // Declaration de variables
    bool procesado = false, opValidacion[max_OP]{};
    int posID = 0, posEval = 0, posExacto = -1;
    char anterior = 0, anteriorDeOp, proximoDeOp;
    // Procesamiento de identificadores
    while(1){
        char letra = archOrigen.get();
        bool haySimilares = false;
        bool huboDescarte = hayDescarteEspecial(archOrigen,letra);
        if(not huboDescarte){
            operando.identificador[posID++] = letra;
            if(esElemento(letra, separadores)) {
                operando.identificador[posID] = 0;
                if(letra == '}') strcat(operando.identificador, ";");
                break;
            } else if(esElemento(letra, 0, agrupadores)){
                procesado = true;
                almacenarHastaDelimitador(archOrigen, operando.identificador, obtenerAgrupadorInverso(letra), posID);
            } else{
                for(int posOp=0;operators[posOp].identificador[0];posOp++){
                    if(opValidacion[posOp] or posEval == 0){
                        if(operators[posOp].identificador[posEval] == letra){
                            if(operators[posOp].identificador[posEval+1]=='\0'){
                                proximoDeOp = archOrigen.get();
                                if(operators[posOp].esAcotable or not(esLetra(anteriorDeOp) or esLetra(proximoDeOp))) posExacto = posOp;
                                archOrigen.unget();
                            }
                            opValidacion[posOp] = true;
                            haySimilares = true;
                        } else opValidacion[posOp] == false;
                    }
                }
                posEval++;
            }
        }
        if (not haySimilares){
            if(posExacto != -1){
                if(huboDescarte) operando.identificador[posID] = 0;
                else operando.identificador[--posID] = 0;
                archOrigen.unget();
                if(operators[posExacto].esSegmentador) espaciarOperadorEnCadena(operators[posExacto].identificador, operando.identificador, false);
                break;
            } else if(posEval > 1){
                if(not procesado){
                    archOrigen.unget();
                    posID--;
                } else procesado = false;
            }
            posEval = 0;
            anteriorDeOp = letra;
        }
        anterior = letra;
    }
}

                       /* - / Funciones Auxiliares / - */

// Modulo de apertura de archivos 'ifstream'
ifstream abrirArchivo_IFS(const char *nombArch){
    ifstream archIFS(nombArch, ios::in);
    fstream archFS(nombArch, ios::in|ios::out);
    archFS.seekg(-1,ios::end);
    if(archFS.get() != '\n') archFS<<endl;
    return archIFS;
}
// Modulo de apertura de archivos 'ofstream'
ofstream abrirArchivo_OFS(const char *nombArch){
    ofstream archOFS(nombArch, ios::out | ios::trunc);
    return archOFS;
}
// Modulo de validacion de simbolo como 'Letra'
bool esLetra(char simbolo){
    return (simbolo >= 'A' and simbolo <= 'z');
}
// Modulo de validacion de simbolo como 'Numero'
bool esNumero(char simbolo){
    return (simbolo >= '0' and simbolo <= '9');
}
// Modulo de validacion de cadena como 'Keyword'
bool esPalabraClave(char *cadena){
    for(int i = 0; keywords[i].identificador[0]; i++) if(strcmp(keywords[i].identificador, cadena) == 0) return true;
    return false;
}
// Modulo de validacion de simbolo como elemento de un conjunto 'char'
bool esElemento(char simbolo, const char *conjunto){
    for(int i = 0; conjunto[i]; i++) if(conjunto[i] == simbolo) return true;
    return false;
}
// Modulo de validacion de simbolo como elemento en posición específica de un conjunto 'char *'
bool esElemento(char simbolo, int j, const char **conjunto){
    for(int i = 0; conjunto[i]; i++) {
        if(conjunto[i][j] == simbolo) return true;
    }
    return false;
}
// Modulo validacion y procesamiento de descarte especial
bool hayDescarteEspecial(ifstream &archOrigen, char letra){
    if(esElemento(letra, espaciadores)) archOrigen>>ws;
    else if (hayDescarteDeComentario(archOrigen, letra));
    else if(letra == '#') while(archOrigen.get() != '\n');
    else return false;
    return true;
}
// Modulo validacion y procesamiento de descarte de comentario
bool hayDescarteDeComentario(ifstream &archOrigen, char letra){
    if(letra == '/'){
        letra = archOrigen.get();
        if (letra == '/') while (archOrigen.get() != '\n');
        else if (letra == '*') {
            while (1) {
                while (archOrigen.get() != '*' and not archOrigen.eof());
                if (archOrigen.eof() or archOrigen.get() == '/') break;
            }
        } else{
            archOrigen.unget();
            return false;
        }
        return true;
    }
    return false;
}
// Modulo de retorno de agrupador inverso
char obtenerAgrupadorInverso(char simbolo) {
    for (int i = 0; agrupadores[i][0]; i++){
        if (agrupadores[i][0] == simbolo) return agrupadores[i][1];
        if (agrupadores[i][1] == simbolo) return agrupadores[i][0];
    }
    return 0;
}
// Modulo de retorno de cadena exacta en memoria dinámica
char *obtenerDinamicoExacto(const char *cadena) {
    char *dinamicoExacto = new char[strlen(cadena) + 1] {};
    strcpy(dinamicoExacto, cadena);
    return dinamicoExacto;
}
// Modulo de concatenacion de cadenas por tipo de elemento ['Keyword' || 'Identifier']
void concatenarPorTipoDeElemento(char *cadDestino, char *cadOrigen, char tipo){
    int medida = strlen(cadDestino);
    if(medida > 0){
        if((tipo == 'K' and !(esElemento(cadDestino[medida - 1], modificadores))) or tipo == 'I'){
            strcat(cadDestino," ");
        }
    }
    strcat(cadDestino, cadOrigen);
}
// Modulo de Descarte de Datos por Tipo de Declaration
void descartarProximosDatosDeDeclaracion(ifstream &archOrigen, char tipo){
    if(tipo == 'F') {
        if(not pf.procesarFunciones) descartarHastaDelimitador(archOrigen,')');
        archOrigen>>ws;
        if(archOrigen.get() == '{') descartarHastaDelimitador(archOrigen,'}');
    } else descartarHastaDelimitador(archOrigen,';');
}
// Modulo de descarte de caracteres hasta delimitador
void descartarHastaDelimitador(ifstream &archOrigen, char delimitador){
    char letra = 0;
    while (1) {
        letra = archOrigen.get();
        if (letra == delimitador or (esElemento(letra, separadores) and !esElemento(delimitador, 1, agrupadores))) break;
        if ((delimitador != 39 and delimitador != '"') and (esElemento(letra, 0, agrupadores) and letra != archOrigen.get())){
            archOrigen.unget();
            letra = obtenerAgrupadorInverso(letra);
            descartarHastaDelimitador(archOrigen,letra);
        }
    }
}
// Modulo de almacenamiento de caracteres hasta delimitador en cadena desde posición específica
void almacenarHastaDelimitador(ifstream &archOrigen, char *cadena, char delimitador, int &posCad){
    bool existeLetra = false;
    int medContenido = 0;
    char letra = 0;
    while (1){
        letra = archOrigen.get();
        if(not hayDescarteDeComentario(archOrigen, letra)){
            if (letra != '\n'){
                medContenido++;
                if (delimitador == '"' or not esElemento(letra, espaciadores)){
                    cadena[posCad++] = letra;
                    if (letra != delimitador) existeLetra = true;
                }
                if(delimitador != 39 and delimitador != '"') {
                    if(delimitador == ')' and letra == ',' and pf.espaciarSubelementos) {
                        cadena[posCad++] = ' ';
                    }
                    if (esElemento(letra, 0, agrupadores) and letra != archOrigen.get()){
                        archOrigen.unget();
                        almacenarHastaDelimitador(archOrigen, cadena, obtenerAgrupadorInverso(letra), posCad);
                    }
                }
            }
            if(letra == delimitador){
                if(delimitador == 39 and medContenido > 1 and not existeLetra){
                    cadena[posCad-1] = ' ';
                    cadena[posCad++] = delimitador;
                }
                cadena[posCad] = 0;
                break;
            }
        }
    }
}
// Modulo almacenamiento de proximos modificadores de palabra clave
void almacenarProximosModificadores(ifstream &archOrigen, char *cadena){
    int medida = strlen(cadena);
    char letra;
    for (int i = medida;1;i++){
        archOrigen>>ws;
        letra = archOrigen.get();
        if (not esElemento(letra,modificadores)){
            cadena[i] = 0;
            archOrigen.unget();
            break;
        }
        if (i == medida) cadena[i++] = ' ';
        cadena[i] = letra;
    }
}
// Modulo separacion de operador y operando en cadena
void espaciarOperadorEnCadena(const char *op,char *cad,bool espaciarAlFinal){
    int medOperador = strlen(op), posCad = strlen(cad) - medOperador;
    cad[posCad++] = ' ';
    cad[posCad] = 0;
    strcat(cad,op);
    if(espaciarAlFinal) strcat(cad," ");
}
// Modulo de Emision de Errores Comunes
void darWarning(char warningID,const char *reason = ""){
    switch (warningID){
        case 'A':   // A -> Archive Aperture
            // cout<<"[ ERROR DE APERTURA ]"<<endl;
            // cout<<"No se encontro el archivo '"<<reason<<"' en el directorio.";
            // cout<<endl<<endl<<"[#] Acciones recomendadas:"<<endl;
            // cout<<"   [A] Verificar la ruta del archivo."<<endl;
            // cout<<"   [B] Verificar el nombre del archivo ingresado."<<endl;
            // cout<<"   [C] Verificar si se agrego la extension del archivo.";
            // cout<<endl;
            break;
        case 'E':   // E -> Empty
            // cout<<"[ SIN RESULTADOS ]"<<endl;
            // cout<<endl<<"No existe error como tal.."<<endl;
            // cout<<"Esto solo signifca que no hay nada para convertir."<<endl;
            // cout<<endl<<"[#] Acciones recomendadas:"<<endl;
            // cout<<"   [A] Activar alguno de los controladores de muestra.";
            // cout<<endl<<"   [B] Editar el archivo fuente."<<endl;
            break;
        case 'L':   // L -> Limit
            // cout<<"[ SIN AJUSTE A LIMITE ]"<<endl;
            // cout<<endl<<"No existe error como tal.."<<endl;
            // cout<<"No obstante, fue imposible acomodar algunas declaraciones";
            // cout<<endl<<"respecto al margen de pagina ['"<<pf.limitePorMargen;
            // cout<<"']. Por ello, se"<<endl<<"ignoro el ajuste hacia ";
            // cout<<"margen en estas declaraciones."<<endl;
            // cout<<"Primera Ubicacion: "<<reason<<endl<<endl;
            // cout<<"[#] Acciones recomendadas:"<<endl;
            // cout<<"   [A] Incrementar el limite de margen."<<endl;
            // cout<<"   [B] Editar el archivo Fuente"<<endl;
            // cout<<"   [C] Desactivar el controlador de ajuste a margen."<<endl;
            break;
        case 'O':   // O -> Order
            // cout<<"ERROR POR ORDENAMIENTO";
            // cout<<endl<<endl<<"El tipo de ordenamiento '";
            // cout<<reason<<"' definido en el controlador es ";
            // cout<<endl<<"invalido."<<endl<<"[#] Acciones recomendadas:"<<endl;
            // cout<<"[A] Modificar el valor del controlador de tipo de";
            // cout<<endl<<"    ordenamiento a alguno de los tipos predefinidos:";
            // cout<<endl<<"    ['A'] Ascendente | ['C'] Consecuente | ";
            // cout<<"['D'] Descendente"<<endl;
            // cout<<"    Recordar que la secuencia debe ser de unicamente"<<endl;
            // cout<<"    '3' caracteres, y que la posicion de cada criterio de";
            // cout<<"ordenamiento es:"<<endl;
            // cout<<"        {Tipo de Declaration}{KeyWords}{Identificadores}";
            // cout<<endl<<"    Por ejemplo, con la secuencia 'ADA' el ";
            // cout<<"ordenamiento sería:"<<endl;
            // cout<<"    - Ascendente por Tipo de Declaration"<<endl;
            // cout<<"    - Descendente por Keyword"<<endl;
            // cout<<"    - Ascendente por Identificador"<<endl;
            break;
        case 'P':   // P -> Partition
            // cout<<"[ ERROR POR PARTICION ]"<<endl;
            // cout<<endl<<"Se ha detectado la partición de un identificador.";
            // cout<<endl<<"Ubicacion: "<<reason<<endl<<endl;
            // cout<<"[#] Acciones recomendadas:"<<endl;
            // cout<<"   [A] Agregar una palabra clave faltante en el";
            // cout<<" diccionario"<<endl<<"       respectivo."<<endl;
            // cout<<"   [B] Editar el archivo fuente."<<endl;
            break;
        case 'S':   // S -> Soon
            // cout<<"[ COMING SOON ]"<<endl;
            // cout<<"Has descubierto una funcionalidad que aun se esta ";
            // cout<<"preparando.. * fallece *"<<endl;
            break;
    }
    exit(1);
}
// Modulo de limpieza de lista de declaraciones
void limpiarListaDeDeclaraciones() {
    Node *pAux = ldx.inicial, *pAnt;
    while(pAux != nullptr) {
        delete[] pAux->declaracion->palabraClave;
        delete[] pAux->declaracion->identificador;
        if(pAux->declaracion->funcion != nullptr) {
            for(int i = 0; i < pAux->declaracion->funcion->numParametros; i++) {
                delete[] pAux->declaracion->funcion->parametros[i].palabraClave;
                delete[] pAux->declaracion->funcion->parametros[i].identificador;
            }
            delete[] pAux->declaracion->funcion->parametros;
            delete pAux->declaracion->funcion;
        }
        if(pAux->declaracion->asignacion != nullptr) {
            for(int i = 0; i < pAux->declaracion->asignacion->numOperandos; i++) {
                delete[] pAux->declaracion->asignacion->operandos[i].identificador;
            }
            delete[] pAux->declaracion->asignacion->operandos;
            delete pAux->declaracion->asignacion;
        }
        delete pAux->declaracion;
        pAnt = pAux;
        pAux = pAux->proximo;
        delete pAnt;
    }
    ldx.inicial = nullptr;
    ldx.final = nullptr;
}
// Modulo de impresion de lista de declaraciones
void imprimirListaDeDeclaraciones(ofstream &archSalida){
    Node *pAux = ldx.inicial;
    while (pAux != nullptr){
        int posApertura = strlen(pAux->declaracion->palabraClave) + strlen(pAux->declaracion->identificador);
        archSalida<<endl<<pAux->declaracion->palabraClave;
        if(not esElemento(pAux->declaracion->palabraClave[strlen(pAux->declaracion->palabraClave) - 1], modificadores)){
            archSalida<<' ';
            posApertura++;
        }
        archSalida<<pAux->declaracion->identificador;
        switch(pAux->declaracion->tipo){
            case 'A':
                imprimirAsignacion(archSalida, *(pAux->declaracion->asignacion), posApertura);
                break;
            case 'F':
                imprimirFuncion(archSalida, *(pAux->declaracion->funcion), posApertura);
                break;
            default:
                darWarning('S');
        }
        pAux = pAux->proximo;
    }
}
// Modulo de impresion de funcion
void imprimirFuncion(ofstream &archSalida, Function funcion, int posApertura){
    int numParametros = funcion.numParametros;
    int posColumna = posApertura, posConjunta;
    bool existeID, existeMD, separarElementos;
    for(int p = 0; p < numParametros; p++){
        Parameter parametro = funcion.parametros[p];
        existeMD = esElemento(parametro.palabraClave[strlen(parametro.palabraClave) - 1], modificadores);
        existeID = (strlen(parametro.identificador) > 1 + pf.espaciarSubelementos);
        separarElementos = existeID and not existeMD;
        posConjunta = strlen(parametro.palabraClave) + strlen(parametro.identificador);
        if(separarElementos) posConjunta++;
        if(p == numParametros - 1) posConjunta++;
        if(seAjustaPorMargen(archSalida,posApertura, posConjunta, posColumna) and p == 0){
            archSalida.seekp(-1*(posApertura+2),ios::cur);
        }
        archSalida<<parametro.palabraClave;
        if(separarElementos) archSalida.put(' ');
        archSalida<<parametro.identificador;
    }
    archSalida<<pf.simboloDelimitador<<endl;
}
// Modulo de impresion de asignacion
void imprimirAsignacion(ofstream &archSalida,Assignment asignacion, int posApertura){
    int numOperandos = asignacion.numOperandos;
    int posColumna = posApertura, posConjunta;
    if(asignacion.esAgrupada){
        posColumna++;
        archSalida<<" {";
        for(int op = 0;op < numOperandos;op++){
            Operand operando = asignacion.operandos[op];
            posConjunta = strlen(operando.identificador);
            if(seAjustaPorMargen(archSalida,posApertura, posConjunta, posColumna) and op == 0){
                archSalida.seekp(-1*(posApertura+2),ios::cur);
            }
            archSalida<<operando.identificador;
        }
    } else{
        for(int op = 0;op < numOperandos;op++){
            Operand operando = asignacion.operandos[op];
            posConjunta = strlen(operando.identificador);
            if(seAjustaPorMargen(archSalida,posApertura,1 + posConjunta, posColumna) and op == 0){
                archSalida.seekp(-1*(posApertura+2),ios::cur);
            }
            archSalida<<' '<<operando.identificador;
        }
    }
    archSalida<<endl;
}
// Modulo de ajuste por margen
bool seAjustaPorMargen(ofstream &archSalida,int posApertura,int posConjunta,
                       int &posColumna){
    if(pf.ajustarPorMargen and posColumna + posConjunta > pf.limitePorMargen){
        archSalida<<endl<<setw(posApertura)<<' ';
        posColumna = posApertura + posConjunta;
        return true;
    }
    posColumna += posConjunta;
    return false;
}
// Modulo de insercion de ordenada de declaracion en lista
void insertarDeclaracionEnLista(Declaration declaracion){
    Node *pAux = ldx.inicial, *pAnt = pAux, *pNuevo = new Node {new Declaration {declaracion}, nullptr};
    if(ldx.inicial == nullptr){
        ldx.inicial = pNuevo;
        ldx.final = pNuevo;
    } else {
        if(pf.ordenarDeclaraciones){
            while(pAux != nullptr){
                if(seInsertaAntesDeNodo(declaracion, *(pAux->declaracion))){
                    if(pAnt == pAux){
                        pNuevo->proximo = ldx.inicial;
                        ldx.inicial = pNuevo;
                    } else{
                        pAnt->proximo = pNuevo;
                        pNuevo->proximo = pAux;
                    }
                    break;
                }
                pAnt = pAux;
                pAux = pAux->proximo;
            }
        } else pAux = nullptr;
        if(pAux == nullptr){
            ldx.final->proximo = pNuevo;
            ldx.final = pNuevo;
        }
    }
}
// Modulo de comparacion de declaraciones para insercion
bool seInsertaAntesDeNodo(Declaration declaracion, Declaration d_Aux){
    int diff_DT,diff_KW,diff_ID;
    diff_DT = declaracion.tipo - d_Aux.tipo;
    diff_KW = strcmp(declaracion.palabraClave, d_Aux.palabraClave);
    diff_ID = strcmp(declaracion.identificador, d_Aux.identificador);
    for (int i = 0;i < 3;i++) {
        switch (pf.criteriosDeOrdenamiento[i]){
            case 'A':
                if ((i == 0 and diff_DT < 0) or
                    (i == 1 and diff_DT == 0 and diff_KW < 0) or
                    (i == 2 and diff_DT == 0 and diff_KW == 0 and diff_ID < 0))
                    return true;
                break;
            case 'C':
                if(i == 0) diff_DT = 0;
                if(i == 1) diff_KW = 0;
                break;
            case 'D':
                if ((i == 0 and diff_DT > 0) or
                    (i == 1 and diff_DT == 0 and diff_KW > 0) or
                    (i == 2 and diff_DT == 0 and diff_KW == 0 and diff_ID > 0))
                    return true;
                break;
            default:
                darWarning('O',pf.criteriosDeOrdenamiento);
        }
    }
    return false;
}
