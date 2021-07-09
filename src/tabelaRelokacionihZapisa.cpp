#include<list>
#include<iostream>
using namespace std;
#include "tabelaRelokacionihZapisa.h"

TabelaRelokacionihZapisa::TabelaRelokacionihZapisa(int brSekcije) {
    this->listaRelokacionihZapisa = list<RelokacioniZapis>();
    this->brSekcije = brSekcije; //sekcija za koju je tabela rel zapisa
}

TabelaRelokacionihZapisa::TabelaRelokacionihZapisa() {
    this->listaRelokacionihZapisa = list<RelokacioniZapis>();
}

void TabelaRelokacionihZapisa::dodajRelokacioniZapis(RelokacioniZapis rz) {
    listaRelokacionihZapisa.push_back(rz);
}

int TabelaRelokacionihZapisa::dohvatiSekciju() {
    return this->brSekcije;
}

list<RelokacioniZapis> TabelaRelokacionihZapisa::dohvatiListuRelZapisa() {
    return this->listaRelokacionihZapisa;
}
