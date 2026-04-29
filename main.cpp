
/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  converter.cpp
 * Autor:               CyndxTs o.0?!
/*/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#include "converter.h"

// Modulo Principal
int main(int argc, char** argv) {
    //
    ifstream archOrig = abrirArchivo_IFS("CodeSource.txt");
    ofstream archDest = abrirArchivo_OFS("HeaderConversion.txt");
    // Proceso de Transcripcion Hacia Cabezera
    HeaderConversion(archOrig, archDest);

    return 0;
}
