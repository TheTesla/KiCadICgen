#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Table.h"
#include "ICSymbol.h"
#include "ICProcTable.h"
#include <ctime>
#include <iomanip>
#include <stdio.h>
#include "KiCadlibOp.h"

using namespace std;



string getKiCadlibPathfromconf(string confFilename)
{
    Table tab;
    ifstream confFile;
    string path;
    char lastchr; 
    confFile.open(confFilename.c_str());
    tab.loadTable(confFile);
    tab.eraseemptyrows();
    path = tab.Tableread(tab.findrow("KICAD_LIB_PATH"),1);
    lastchr = path[path.length()-1];
    if(lastchr!='\\') path.append("\\");
    return path;
}

void addICSymbol(string pinFilename, string confFilename)
{
    ICdevSym Sym1;
    All4PinDescrVec AllPins;
    ICpinSym* SymPin;
    string teststr;
    stringstream newlibEntry;
    ifstream pinFile;
    ifstream confFile;
    Table pinTab;
    string libFilename;
    ICOptions Opt;
    pinFile.open(pinFilename.c_str());
    confFile.open(confFilename.c_str());

    pinTab.loadTable(pinFile);
    pinTab.eraseemptyrows();
    
    Sym1 = readProperties(pinTab);
    AllPins = readPinDescr(pinTab);
    Opt = readOptions(pinTab);
    
    pinFile.close();
    confFile.close();  
  
    // Alle Pins (mit Koordinaten) am Symbol sind zusamegfasst.
    SymPin = new ICpinSym[AllPins.numbPinsR+AllPins.numbPinsL+AllPins.numbPinsU+AllPins.numbPinsD];
    Sym1.Pincount = 0;
    Sym1.Pins = SymPin;

    updateICSymSize(Sym1, AllPins.PDVec_R, AllPins.numbPinsR, 'R');
    updateICSymSize(Sym1, AllPins.PDVec_L, AllPins.numbPinsL, 'L');
    updateICSymSize(Sym1, AllPins.PDVec_U, AllPins.numbPinsU, 'U');
    updateICSymSize(Sym1, AllPins.PDVec_D, AllPins.numbPinsD, 'D');
    ICSymPinPlace(Sym1, AllPins.PDVec_R, AllPins.numbPinsR, 'R', 200);
    ICSymPinPlace(Sym1, AllPins.PDVec_L, AllPins.numbPinsL, 'L', 200);
    ICSymPinPlace(Sym1, AllPins.PDVec_U, AllPins.numbPinsU, 'U', 200);
    ICSymPinPlace(Sym1, AllPins.PDVec_D, AllPins.numbPinsD, 'D', 200);

    if(+1 != Opt.stepsize){
        roundICcoords(Sym1, AllPins.numbPinsR+AllPins.numbPinsL+AllPins.numbPinsU+AllPins.numbPinsD, Opt.stepsize);
    }

    libFilename = getKiCadlibPathfromconf(confFilename)+Sym1.Library+".lib";
    newlibEntry << ICSymprint(Sym1);
    if(checkKiCadlibexists(libFilename)){
        cout << "Adding new entry: " << Sym1.name << " to " << libFilename << endl;
        KiCadlibRemoveSym(libFilename, Sym1.name);
        KiCadlibInsertSym(libFilename, Sym1.name, newlibEntry.str());
    }
    else{
        // kein lib-Header vorhanden
        cout << "Adding new lib with new header" << endl;
        cout << "Adding new entry: " << Sym1.name << " to " << libFilename << endl;
        KiCadinitlib(libFilename, newlibEntry.str());
    }
}



int main(int argc, char *argv[])
{
    string confFilename;
    string arg;
    string libFilename;
    int i;


    cout << endl << "Copyright (C) 2012 Stefan Helmert <stefan.helmert@gmx.net>" << endl << endl;
    
    if(1 == argc){
        cout << "KiCad library-generator updates or generates KiCad .lib-files from .pin-file" << endl << endl;
        cout << "KiCad-Library-Path can be specified in .conf-file, example:" << endl;
        cout << "    KICAD_LIB_PATH,  C:\\Program Files (x86)\\KiCad\\share\\library\\" << endl << endl;
        cout << "Usage: KiCadICgen [config.conf] pinfile.pin [2ndpinfile.pin ...]" << endl;
        return EXIT_SUCCESS;
    }
    
    // conf-Datei angegeben
    confFilename = "KiCadICgen.conf"; // Standard
    for(i=1;i<argc;i++){
        arg = argv[i];
        if(-1 != arg.find(".conf")){
            confFilename = arg;
            break;
        }
    }
    
    cout << "Using configuration File: " << confFilename << endl;
    
    // alle angegeben pin-Files verarbeiten
    for(i=1;i<argc;i++){    
        arg = argv[i];
        if(-1 != arg.find(".pin")){
            cout << "Processing: " << arg << endl;
            addICSymbol(arg.c_str(),confFilename.c_str());
        }
    }
    
    return EXIT_SUCCESS;
}
