#include<string>
#include "string.h"
#include "relokacioniZapis.h"
using namespace std;

RelokacioniZapis::RelokacioniZapis(string imeSimbola, int pomeraj, int tip, int redniBroj, int velicina) {
    this->imeSimbola = imeSimbola;
    this->pomeraj = pomeraj;
    this->tip = tip;
    this->redniBroj = redniBroj; //ovo je redni broj iz tabele simbola
    this->velicina = velicina;
}

string RelokacioniZapis::dohvatiImeSimbola() {
    return this->imeSimbola;
}
void RelokacioniZapis::postaviRedniBroj(int vr) {
    this->redniBroj = vr;
}
int RelokacioniZapis::dohvatiPomeraj() {
    return this->pomeraj;
}
int RelokacioniZapis::dohvatiTip() {
    return this->tip;
}
int RelokacioniZapis::dohvatiRedniBroj() {
    return this->redniBroj;
}
int RelokacioniZapis::dohvatiVelicinu() {
    return this->velicina;
}
