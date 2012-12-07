
#ifndef KICADLIBOP_H
#define KICADLIBOP_H

#include "ICSymbol.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;

string ICSymprint(ICdevSym Sym);
void KiCadlibRemoveSym(string libFilename, string Symname);
void KiCadlibInsertSym(string libFilename, string Symname, string content);
int checkKiCadlibexists(string libFilename);
void KiCadinitlib(string libFilename, string content);

#endif
