#include<iostream>
#include<string>
#include<list>
using namespace std;

struct Pojavljivanje {
    int velicina;
    int sekcija;
    int adresa; //pomeraj u okviru sekcije
    int pomeraj;
    int velicinaOperanada;
    int vrednostPCja;
    Pojavljivanje(int sekcija, int adresa, int velicina) {
        this->velicina = velicina;
        this->adresa = adresa;
        this->sekcija = sekcija;
        this->pomeraj = 0;
        this->velicinaOperanada = 0;
        this->vrednostPCja = 0;
    }
    Pojavljivanje(int sekcija, int adresa, int velicina, int pomeraj, int velicinaOperanada, int vrednostPCja) {
        this->velicina = velicina;
        this->adresa = adresa;
        this->sekcija = sekcija;
        this->pomeraj = pomeraj;
        this->velicinaOperanada = velicinaOperanada;
        this->vrednostPCja = vrednostPCja;
    }
};


class Simbol {

public:

    Simbol(string ime, int sekcija, string vidljivost, int redniBroj, int pomeraj);
    
    
    string ime;
    int sekcija;
    string vidljivost;
    int redniBroj;
    int pomeraj;
    int velicinaSekcije;
    list<Pojavljivanje> listaPojavljivanja;
    bool externSimbol;
    bool globalSimbol;
    bool equSimbol; // false dok se ne nadje u situaciji .equ simbol, vrednost
    int equVrednost; // -1 dok ne dodje do .equ, e onda proveravam da kad ispisujem
                            //ako je .equ, ispisujem ovu vrednost, inace pomeraj
    string equIzraz; //ovo ce se inic kad naidjem na equ, ako naidjem
    bool definisan; //ovo kad ga definisem
    bool mustEqu;
    bool simbolSekcija;
    list<string> equLista;
    

    string dohvatiIme();
    string dohvatiVidljivost();
    int dohvatiSekciju();
    int dohvatiPomeraj();
    int dohvatiRedniBroj();
    void dodajPojavljivanje(int sekcija, int adresa, int velicina);
    //ispis
    void postaviEkstern();
    void postaviGlobal();
    void postaviEquVrednost(int vrednost);
    bool vecDefinisan();
    void postaviDefinisan(int vrednost);
    void postaviEqu(string izraz);
};
