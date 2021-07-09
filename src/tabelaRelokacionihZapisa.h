#include<string>
#include "string.h"
#include<iostream>
#include<list>
#include "relokacioniZapis.h"
using namespace std;

class TabelaRelokacionihZapisa {
public:
    TabelaRelokacionihZapisa();
    TabelaRelokacionihZapisa(int brojSekcije);
    void dodajRelokacioniZapis(RelokacioniZapis rz);
    int brSekcije;
    int dohvatiSekciju();
    list<RelokacioniZapis> dohvatiListuRelZapisa();
    
private:
    list<RelokacioniZapis> listaRelokacionihZapisa;
};
