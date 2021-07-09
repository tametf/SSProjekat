#include<iostream>
#include<list>
#include "simbol.h"

class TabelaSimbola {
public:


    int brojac;

    TabelaSimbola();
    
    void dodajSekciju(Simbol s); //dodaje na pocetak liste sekciju
    void dodajSimbol(Simbol s);
    
    bool postoji(Simbol s);
    bool postoji(string ime);
    int dohvatiSekciju(string imeSimbola);
    int dohvatiRedniBroj(string imeSimbola);
    string dohvatiVidljivost(string imeSimbola);
    int dohvatiPomeraj(string imeSimbola);
    bool promeniVidljivostNaGlobalnu(string imeSimbola);
    Simbol* pronadjiSimbol(string ime);
    Simbol* pronadjiSekciju(int br);
    list<Simbol>& dohvatiSveSimbole();
    string dohvatiNazivSekcije(int br);
    
    void postaviVelicinuSekcije(int redniBrojSekcije, int velicinaSekcije, string imeSekcije);

private:
    list<Simbol> listaSimbola;
};