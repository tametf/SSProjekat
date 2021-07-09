#pragma once

#include<list>
#include<string>
#include<iostream>
#include<string.h>
#include<unordered_map>
#include<vector>

struct Parametar {
    bool pcRel = false;
    bool visiBitiRegistra = false;
    int kodRegistra = -1;
    int kodAdresiranja = -1;
    std::string parametar = "";
    std::string tipAdresiranja = "";
};

class ParserLinije {
public:
    ParserLinije(std::string linija, int brLinije);
    enum TipLinije {Sekcija, Direktiva, Instrukcija, Prazna};   
    
    std::string dohvatiLabelu() const;
    std::string dohvatiDirektivu() const;
    std::string dohvatiInstrukciju() const;
    std::string dohvatiSekciju() const;
    int dohvatiBrojParametara();

    std::list<std::string>& dohvatiVrednosti(); //za .global i .extern .byte .word 
    int dohvatiVrednost() const; //.skip
    std::list<std::string>& dohvatiSimbole();
    std::string dohvatiSimbol() const;
    std::string dohvatiIzraz() const;
    TipLinije dohvatiTipLinije() const;

    std::list<Parametar> dohvatiParametre();

    bool dohvatiJedanBajt();
    
    int dohvatiVelicinuInstrukcije() const;
    unsigned long long int dohvatiKodInstrukcije();


    int dohvatiKodAdresiranja(std::string tipAdresiranja);
    std::string odrediTipAdresiranja(std::string instr);
    std::string odrediTipAdresiranjaSkok(std::string instr);
    int odrediKodRegistra(std::string operand, int brLinije);
    void odrediViseBiteRegistra(std::string operand, int brOp);     
    void parsiraj(std::string linija, int brLinije);
    std::string dohvatiNazivInstrukcije();
private:
    Parametar dohvatiPrviParametar();
    Parametar dohvatiDrugiParametar();
    
    std::string dohvatiPrviParam();
    std::string dohvatiDrugiParam();
    
    bool dohvatiPcRelPrvi();
    bool dohvatiPcRelDrugi();
    bool dohvatiVisiBitiRegistraPrvi();
    bool dohvatiVisiBitiRegistraDrugi();
      
    unsigned long long int dohvatiKodRegistraPrvi(); //kod npr za r1 bice 0x1
    unsigned long long int dohvatiKodRegistraDrugi();
    unsigned long long int dohvatiKodAdresiranjaPrviParam(); //za imm bice 0x0
    unsigned long long int dohvatiKodAdresiranjaDrugiParam();

    std::string dohvatiTipAdresiranjaPrvi();
    std::string dohvatiTipAdresiranjaDrugi();
    
    std::string direktiva;
    std::string instrukcija;
    std::string labela;
    std::string sekcija;
    std::string nazivInstrukcije;
    int brParam;

    std::list<std::string> vrednosti;
    int vrednost;
    std::string simbol;
    std::list<std::string> simboli;
    TipLinije tipLinije;
    std::string izraz;

    bool pcRel;
    //int trenutniParametri;
    bool jedanBajt;
    int velicinaInstrukcije;
    //unsigned long long kodInstrukcije;
    bool pcRelPrvi;
    bool pcRelDrugi;
    bool visiBitiRegistraPrvi;
    bool visiBitiRegistraDrugi;
    std::string tipAdresiranjaPrviParam; //naziv samo
    std::string tipAdresiranjaDrugiParam;  
    unsigned long long int kodRegistraPrvi; //kod npr za r1 bice 0x1
    unsigned long long int kodRegistraDrugi;
    unsigned long long int  kodAdresiranjaPrviParam; //za imm bice 0x0
    unsigned long long int  kodAdresiranjaDrugiParam;  
    unsigned long long int kodInstrukcije;
    std::string prviParam;
    std::string drugiParam;      
};