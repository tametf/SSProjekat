#include<iostream>
#include<string>
#include "relokacioniZapis.h"
using namespace std;


class Sekcija {
public:
    Sekcija(string naziv, int sekcija) {
        this->naziv = naziv;
        this->sekcija = sekcija;
        listaRelokacionihZapisa = list<RelokacioniZapis>();
        this->bajtoviZaIspis = "";
    }
    string dohvatiNaziv() {
        return this->naziv;
    }
    int dohvatiBroj() {
        return this->sekcija;
    }
    list<RelokacioniZapis> dohvatiListuRelZapisa() {
        return this->listaRelokacionihZapisa;
    }
    void dodajRelZapis(RelokacioniZapis rz) {
        this->listaRelokacionihZapisa.push_back(rz);
    }

    string dohvatiBajtoveZaIspis() {
        return this->bajtoviZaIspis;
    }

    void postaviBajtoveZaIspis(string bajtovi) {
        this->bajtoviZaIspis += bajtovi;
    }

    void zameniBajtove(string noviBajtovi) {
        bajtoviZaIspis = "";
        bajtoviZaIspis = noviBajtovi;
    }

private:
    string naziv;
    int sekcija;
    list<RelokacioniZapis> listaRelokacionihZapisa;
    string bajtoviZaIspis;
    int redniBrojSekcije;

};