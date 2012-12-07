
#ifndef ICPROCTABLE_H
#define ICPROCTABLE_H
#include <iostream>
#include "ICSymbol.h"
#include "Table.h"
#include <sstream>

using namespace std;

struct All4PinDescrVec
{
    int numbPinsL;
    int numbPinsR;
    int numbPinsU;
    int numbPinsD;
    PinDescr* PDVec_L;
    PinDescr* PDVec_R;
    PinDescr* PDVec_U;
    PinDescr* PDVec_D;
};

struct ICOptions
{
    unsigned long stepsize;
};

ICdevSym readProperties(Table tab);
ICOptions readOptions(Table tab);
All4PinDescrVec readPinDescr(Table tab);


#endif
