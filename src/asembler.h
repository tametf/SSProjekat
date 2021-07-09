#include <fstream>
#include <unordered_map>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include "tabelaSekcija.h"
#include "tabelaSimbola.h"
#include "parserLinije.h"

using namespace std;

class Asembler{

public:
    Asembler(string ulaz, string izlaz);
    void jedanJediniProlaz();
    static void pocetnaInicijalizacija();
    void sredi(TabelaSimbola ts, int trenutnaSekcija, string imeTrenutneSekcije, int pomeraj);
    void ispisTabeleSimbola(TabelaSimbola);
    void ispisTabelaRelokacija(TabelaSekcija);
    void ispisKoda();
    int ispisiParametar(Parametar,int,int,int,int);
    string littleEndianDvaBajta(string);
    string intToHex(int,int);
    string intToHexDvaBajta(int);
    string intToHexJedanBajt(int);
    int hexToDec(string);
    
    static vector<string> skokovi;
    static unordered_map<string, int> kodoviOp;
    static unordered_map<string, int> kodoviAdr;
    static unordered_map<string, int> kodoviReg;
    
    static vector<string> nulaParamOp;
    static vector<string> jedanParamOp;
    static vector<string> dvaParamOp;
    static string direktive[8];

    ifstream ulaz;
    string izlazIme;
    ofstream izlaz;

    TabelaSimbola tabelaSimbola;
    TabelaSekcija tabelaSekcija;
private:
    int trenutnaSekcija ;
    int pomeraj;
    string imeTrenutneSekcije;
    int lc;
    string bajtoviZaIspis;
};