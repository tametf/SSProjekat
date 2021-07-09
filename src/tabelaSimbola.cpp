#include <string>
#include "string.h"
using namespace std;
#include<iostream>
#include "tabelaSimbola.h"



TabelaSimbola::TabelaSimbola() {
    listaSimbola = list<Simbol>();
    brojac = 0;
}

void TabelaSimbola::dodajSimbol(Simbol s) {
    listaSimbola.push_back(s);
}


//da li ce mi ovo poremetiti redneBrojeve simbola u tabeli simbola???????????????
void TabelaSimbola::dodajSekciju(Simbol s) {
    listaSimbola.push_front(s);
}

bool TabelaSimbola::postoji(Simbol s) {
    for(Simbol simbol: listaSimbola) {
        if(s.ime==simbol.ime && s.redniBroj==simbol.redniBroj 
           && s.sekcija==simbol.sekcija && s.vidljivost==simbol.vidljivost 
           && s.pomeraj==simbol.pomeraj) return true;
    }
    return false;
}

bool TabelaSimbola::postoji(string imeSimbola) {
    for(Simbol simbol: listaSimbola) {
        if(imeSimbola==simbol.ime) return true;
    }
    return false;
}

int TabelaSimbola::dohvatiSekciju(string imeSimbola) {
    for(Simbol simbol: listaSimbola) {
        if(simbol.ime==imeSimbola) {
            return simbol.sekcija;
        }
    }
    return -1;
}

string TabelaSimbola::dohvatiNazivSekcije(int broj) {
    for(Simbol simbol: listaSimbola) {
        if(simbol.redniBroj==broj) {
            return simbol.ime;
        }
    }
    return "";
}


int TabelaSimbola::dohvatiRedniBroj(string imeSimbola) {
    for(Simbol simbol: listaSimbola) {
        if(simbol.ime==imeSimbola) {
            return simbol.redniBroj;
        }
    }
    return -1;
}

    
string TabelaSimbola::dohvatiVidljivost(string imeSimbola) {
    for(Simbol simbol: listaSimbola) {
        if(simbol.ime==imeSimbola) {
            return simbol.vidljivost;
        }
    }
    return "greska";
}

int TabelaSimbola::dohvatiPomeraj(string imeSimbola) {
    for(Simbol simbol: listaSimbola) {
        if(simbol.ime==imeSimbola) {
            return simbol.pomeraj;
        }
    }
    return -1;
}

bool TabelaSimbola::promeniVidljivostNaGlobalnu(string imeSimbola) {
    for(Simbol simbol: listaSimbola) {
        if(simbol.ime==imeSimbola) {
            simbol.vidljivost="global";
            return true;
        }
    }
    return false;
}
   
    
//void postaviGlobal(string simbol) throw(SintaksnaGreska);


Simbol* TabelaSimbola::pronadjiSimbol(string ime) {
    for(Simbol &s: listaSimbola) {
        if(s.ime == ime) {
            return &s;
        }
    }
    return nullptr;
}

Simbol* TabelaSimbola::pronadjiSekciju(int br) {
    for (Simbol& s : listaSimbola) {
        if (s.redniBroj == br) {
            return &s;
        }
    }
    return nullptr;
}

void TabelaSimbola::postaviVelicinuSekcije(int redniBrojSekcije, int velicinaSekcije, string imeSekcije) {
    Simbol *s = pronadjiSekciju(redniBrojSekcije);
    if(s!=nullptr)
    s->velicinaSekcije = velicinaSekcije;
/*     for(Simbol &s: listaSimbola) {
        if(s.dohvatiRedniBroj() == redniBrojSekcije) {
            s.velicinaSekcije = velicinaSekcije;
        }
     }
*/}


list<Simbol>& TabelaSimbola::dohvatiSveSimbole() {
    return listaSimbola;
}
 /*
string TabelaSimbola::toString() {
    stringstream ispis;
    for(int i = 0; i < listaSimbola.size(); i++) {
        ispis << listaSimbola[i];
    }
    ispis.str();
}*/
