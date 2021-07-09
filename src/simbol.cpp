#include<list>
#include "simbol.h"


 Simbol::Simbol(string ime, int sekcija, string vidljivost, int redniBroj, int pomeraj) {
        this->ime=ime;
        this->sekcija=sekcija;
        this->vidljivost=vidljivost;
        this->redniBroj=redniBroj;
        this->pomeraj=pomeraj;
        this->velicinaSekcije=-1; //ako je sekcija u pitanju, onda gledamo i kolika joj je velicina,
        //to postavimo kad dodjemo do kraja sekcije, tada cemo znati
        this->listaPojavljivanja = list<Pojavljivanje>();
        this->externSimbol = false;
        this->globalSimbol = false;
        this->definisan = false;
        this->equIzraz = "";
        this->equVrednost = 0;
        this->mustEqu = false;
        this->simbolSekcija = false;
        this->equSimbol = false;
        this->equLista = list<string>();
    }


string Simbol::dohvatiIme() { return ime; }
int Simbol::dohvatiSekciju() { return sekcija; }
int Simbol::dohvatiPomeraj() { return pomeraj; }
int Simbol::dohvatiRedniBroj() { return redniBroj; }
string Simbol::dohvatiVidljivost() { return vidljivost; }
void Simbol::postaviGlobal() { 
    this->vidljivost = "g"; 
    this->globalSimbol = true; }
void Simbol::dodajPojavljivanje(int sekcija, int adresa, int velicina) {
    Pojavljivanje p = Pojavljivanje(sekcija, adresa, velicina);
    this->listaPojavljivanja.push_back(p);
}
void Simbol::postaviEkstern() {
    this->externSimbol = true; 
}

void Simbol::postaviEquVrednost(int vrednost) {
    this->equVrednost = vrednost;
}
void Simbol::postaviDefinisan(int vrednost) {
    this->definisan = true;
    this->pomeraj = vrednost;
}

bool Simbol::vecDefinisan() {

    if(this->definisan == true) return true;
    return false;
}


void Simbol::postaviEqu(string izraz) {
    this->equIzraz = izraz;
    this->equSimbol = true;
    list<string> elementi = list<string>();
    string prvi;
    int pozicija;

    //secem razmake s pocetka
    if ((pozicija = izraz.find_first_not_of(" ,\t")) != string::npos) {
        izraz = izraz.substr(pozicija, izraz.length() - pozicija);
    }

    //sredi prvi element
    if ((pozicija = izraz.find_first_of("+-")) != string::npos) {
        if (izraz[0] != '-' && izraz[0] != '+') {
            izraz = "+" + izraz;
        }
    }
    else {
        if (izraz[0] != '-' && izraz[0] != '+') {
            prvi = "+" + izraz;
        }
        elementi.push_back(prvi);
        izraz = "";
    }

    int pozicija1;
    int pozicija2;
    string plusS = "+";
    string minusS = "-";

    //sredi ostale elemente
    while (!izraz.empty()) {

        bool plus = false;
        bool minus = false;

        if ((pozicija1 = izraz.find_first_of("+-")) != string::npos) {
            if (izraz[pozicija1] == '+') {
                plus = true;
            }
            if (izraz[pozicija1] == '-') {
                minus = true;
            }

            izraz = izraz.substr(pozicija1 + 1, izraz.length() - pozicija1 - 1);

            if ((pozicija2 = izraz.find_first_of("+-")) != string::npos) {
                string pom = izraz.substr(0, pozicija2);
                int p;
                if ((p = pom.find_first_not_of(" \t")) != string::npos) {
                    pom = pom.substr(p, pom.length() - p);
                }
                if ((p = pom.find_first_of(" \t")) != string::npos) {
                    pom = pom.substr(0, p);
                }
                if (plus) 
                    pom = plusS + pom;
                else
                    pom = minusS + pom;
                izraz = izraz.substr(pozicija2, izraz.length() - pozicija2);
                elementi.push_back(pom);
            }
            else {
                string pom = izraz;
                int p;
                if ((p = pom.find_first_not_of(" \t")) != string::npos) {
                    pom = pom.substr(p, pom.length() - p);
                }
                if ((p = pom.find_first_of(" \t")) != string::npos) {
                    pom = pom.substr(0, p);
                }
                if (plus) pom = plusS + pom;
                else pom = minusS + pom;
                izraz = "";
                elementi.push_back(pom);
            }
        }
        else {
            cout << "GRESKA, FALI + ILI - U IZRAZU" << izraz << "\n";
            return;
        }
    }

    this->equLista = elementi;
}