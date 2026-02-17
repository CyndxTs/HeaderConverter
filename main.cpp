
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
    
    // Proceso de Transcripcion Hacia Cabezera
    HeaderConversion("CodeSource.txt","HeaderConversion.txt");

    return 0;
}
