
/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  declarations.cpp
 * Autor:               CyndxTs
/*/

#include "declarations.h"

const char *agrupadores[] = {"\"\"", "''", "()", "[]", "{}", nullptr};

const char espaciadores[] = {' ', '\t', '\n', 0};

const char modificadores[] = {'&', '*', 0};

const char separadores[] = {',', ')', '}', ';', 0};

Keyword keywords[max_KW] {};

Operator operators[max_OP] {};

LDX ldx {};

ProcessingFormat pf {};
