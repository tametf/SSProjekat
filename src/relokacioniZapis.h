using namespace std;
#include<string>
#include<iostream>

class RelokacioniZapis {

public:
    RelokacioniZapis(string imeSimbola, int pomeraj, int tip, int redniBroj, int velicina); 
    
    string dohvatiImeSimbola();
    void postaviRedniBroj(int vr);
    int dohvatiPomeraj();
    int dohvatiTip();
    int dohvatiRedniBroj();
    int dohvatiVelicinu();

private:
    string imeSimbola;
    int pomeraj; //msm da nije, nego do pojavljivanja
                 //redniBroj ce ti biti ili sekcija(ako je lokalni simbol) 
                 //ili  redniBroj simbola u tabeli simbola ako je simbol ekstern/global
    int tip; //0-obican, 1-pc
    int redniBroj;
    //int sekcija;
    int velicina;
};

