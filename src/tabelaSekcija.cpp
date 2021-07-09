#include<list>
#include "tabelaSekcija.h"

TabelaSekcija::TabelaSekcija() {
    this->listaSekcija = list<Sekcija>();
}

void TabelaSekcija::dodaj(string naziv, int sekcija) {
    Sekcija s = Sekcija(naziv, sekcija);
    listaSekcija.push_back(s);
}

/*Sekcija* TabelaSekcija::dohvatiSekciju(string naziv) {
    for(Sekcija &sekc: listaSekcija) {
        if(sekc.dohvatiNaziv() == naziv) {
            return &sekc;
        }
    }
    return nullptr;
}*/

Sekcija* TabelaSekcija::dohvatiSekciju(int br) {
    for (Sekcija& sekc : listaSekcija) {
        if (sekc.dohvatiBroj() == br) {
            return &sekc;
        }
    }
    return nullptr;
}
