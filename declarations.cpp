
/*/
 * Projecto:            HeaderConverter
 * Nombre del Archivo:  declarations.cpp
 * Autor:               CyndxTs
/*/

#include "declarations.h"

const char *agrupadores[] = {"\"\"", "''", "()", "[]", "{}", 0};

const char espaciadores[] = {' ', '\t', '\n', 0};

const char modificadores[] = {'&', '*', 0};

const char separadores[] = {',', ')', '}', ';', 0};

Keyword keywords[max_KW] = {{"void"}, {"char"}, {"double"}, {"int"}, {"bool"},
                            {"const"}, {"struct"}, {"operator"}, {"private"},
                            {"public"}, {"ifstream"}, {"ofstream"},
                            {"fstream"}, {"istream"}, {"ostream"}};

Operator operators[max_OP] = {{"+", true, true}, {"-", true, true},
                              {"*", true, false}, {"/", true, false},
                              {":", true, false}, {">", true, true},
                              {"<", true, true}, {">=", true, true},
                              {"<=", true, true}, {"==", true, true},
                              {"!=", true, true}, {"!", true, false},
                              {"||", true, true}, {"&&", true, true},
                              {"or", false, true}, {"and", false, true},
                              {"xor", false, true}};

LDX ldx {};

ProcessingFormat pf {true, 80, true, "ADA", true, true, true, false, ';'};
