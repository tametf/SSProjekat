#include<iostream>
using namespace std;
#include <list>
#include "sekcija.h"

class TabelaSekcija {
public:
    TabelaSekcija();
    void dodaj(string naziv, int sekcija);
    //Sekcija* dohvatiSekciju(string naziv);
    Sekcija* dohvatiSekciju(int br);
    list<Sekcija> dohvatiListu() {
        return listaSekcija;
    }
private:
    list<Sekcija> listaSekcija;
};