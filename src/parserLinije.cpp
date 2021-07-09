#include "parserLinije.h"
#include "asembler.h"

ParserLinije::ParserLinije(string linija, int brLinije) {
    labela="";
    sekcija="";
    direktiva="";
    instrukcija="";
    izraz="";
    brParam = -1;
    kodInstrukcije = -1;
    
    parsiraj(linija, brLinije);
}

Parametar ParserLinije::dohvatiPrviParametar() {
    Parametar p = Parametar();
    p.parametar = dohvatiPrviParam();
    p.tipAdresiranja = dohvatiTipAdresiranjaPrvi();
    p.kodAdresiranja = (int) dohvatiKodAdresiranjaPrviParam();
    p.kodRegistra = (int) dohvatiKodRegistraPrvi();
    p.pcRel = dohvatiPcRelPrvi();
    p.visiBitiRegistra = dohvatiVisiBitiRegistraPrvi();
    return p;
}

Parametar ParserLinije::dohvatiDrugiParametar() {
    Parametar p = Parametar();
    p.parametar = dohvatiDrugiParam();
    p.tipAdresiranja = dohvatiTipAdresiranjaDrugi();
    p.kodAdresiranja = (int) dohvatiKodAdresiranjaDrugiParam();
    p.kodRegistra = (int) dohvatiKodRegistraDrugi();
    p.pcRel = dohvatiPcRelDrugi();
    p.visiBitiRegistra = dohvatiVisiBitiRegistraDrugi();
    return p;
}

list<Parametar> ParserLinije::dohvatiParametre() {
    list<Parametar> parametri = list<Parametar>();
    if (brParam > 0) {
        parametri.push_back(dohvatiPrviParametar());
    }
    if (brParam > 1) {
        parametri.push_back(dohvatiDrugiParametar());
    }
    return parametri;
}

int ParserLinije::dohvatiBrojParametara() {
    return brParam;
}

string ParserLinije::dohvatiNazivInstrukcije() {
    return nazivInstrukcije;
}

bool ParserLinije::dohvatiPcRelPrvi() {
    return pcRelPrvi;
}

bool ParserLinije::dohvatiPcRelDrugi() {
    return pcRelDrugi;
}

bool ParserLinije::dohvatiVisiBitiRegistraPrvi() {
    return visiBitiRegistraPrvi;
}

bool ParserLinije::dohvatiVisiBitiRegistraDrugi() {
    return visiBitiRegistraDrugi;
}

unsigned long long int ParserLinije::dohvatiKodRegistraPrvi() { return kodRegistraPrvi; } //kod npr za r1 bice 0x1
unsigned long long int ParserLinije::dohvatiKodRegistraDrugi() {return kodRegistraDrugi; }
unsigned long long int ParserLinije::dohvatiKodAdresiranjaPrviParam() { return kodAdresiranjaPrviParam; } //za imm bice 0x0
unsigned long long int ParserLinije::dohvatiKodAdresiranjaDrugiParam() { return kodAdresiranjaDrugiParam; }  
unsigned long long int ParserLinije::dohvatiKodInstrukcije() { return kodInstrukcije; }
string ParserLinije::dohvatiTipAdresiranjaPrvi() { return tipAdresiranjaPrviParam; }
string ParserLinije::dohvatiTipAdresiranjaDrugi() { return tipAdresiranjaDrugiParam; }
string ParserLinije::dohvatiPrviParam() { return prviParam; }
string ParserLinije::dohvatiDrugiParam() { return drugiParam; } 

string ParserLinije::dohvatiIzraz() const {
    return izraz;
}

string ParserLinije::dohvatiLabelu() const {
    return labela;
}
string ParserLinije::dohvatiDirektivu() const {
    return direktiva;
}
string ParserLinije::dohvatiInstrukciju() const {
    return instrukcija;
}
string ParserLinije::dohvatiSekciju() const {
    return sekcija;
}
list<string>& ParserLinije::dohvatiVrednosti() {
    return vrednosti;
}

list<string>& ParserLinije::dohvatiSimbole() {
    return simboli;
}

int ParserLinije::dohvatiVrednost() const {
    return vrednost;
}
   
string ParserLinije::dohvatiSimbol() const {
    return simbol;
}
ParserLinije::TipLinije ParserLinije::dohvatiTipLinije() const {
    return tipLinije;
}

int ParserLinije::dohvatiVelicinuInstrukcije() const {
    return velicinaInstrukcije;
}


bool ParserLinije::dohvatiJedanBajt() {
    return jedanBajt;
}

bool jesteSkok(string naziv) {
    for(string skok: Asembler::skokovi) {
        if(skok == naziv) {
            return true;
        }
    }
    return false;
}


string ParserLinije::odrediTipAdresiranja(string instr) {
    pcRel = false;
    int pom;
    if((pom = instr.find_first_not_of(" ,\t")) != string::npos) {
        cout << "SUBSTR " << pom << " " << instr << "\n";
        instr = instr.substr(pom, instr.length()-pom);
    }
    if(instr[0] == '$') {
        return "imm";
    }else if(instr[0] == '%') {
        return "regdir";
    } else if(instr[0] == '(') {
        return "regind";
    } else {
        int pom;
        if((pom = instr.find_first_of("(%")) != string::npos) {
                if(pom = instr.find_first_of("p") != string::npos) {
                    pcRel = true;
                }
                if(pom = instr.find_first_of("7") != string::npos) {
                    pcRel = true;
                }
                return "regind16";           
        }
    }
    return "mem";
}

int ParserLinije::dohvatiKodAdresiranja(string naziv) {
    return (int) Asembler::kodoviAdr.at(naziv);
}


string ParserLinije::odrediTipAdresiranjaSkok(string instr) {
    pcRel = false;
    if(instr[0] == '*' && instr[1] == '%') {
        return "regdir";
    } else if(instr[0] == '*' && instr[1] == '(') {
        return "regind";
    } else {
        int pom;
        if((pom = instr.find_first_of("(%")) != string::npos) {        
                if(pom = instr.find_first_of("p") != string::npos) {
                    pcRel = true;
                }
                if(pom = instr.find_first_of("7") != string::npos) {
                    pcRel = true;
                }
                return "regind16";
            } 
       }
    if(instr[0] == '*') return "mem";
    return "imm";
}

int ParserLinije::odrediKodRegistra(string operand, int brLinije) {
    int pozicija = 0;
    int pozicija2 = 0;
    int pozicija3 = 0;
    int pom = 0;
    if((pom = operand.find_first_of("(")) != string::npos) {
        operand = operand.substr(pom + 1, operand.length() - pom - 1);
    }
    if((pozicija=operand.find_first_of("r")) != string::npos || (pozicija3 = operand.find_first_of("s")) != string::npos || (pozicija2=operand.find_first_of("p")) != string::npos) {
        if (operand.length() > 0) {
            if (pozicija >= 0 || pozicija2 >= 0 || pozicija3 >= 0) {
                if (pozicija >= 0) {
                    if (operand[pozicija] == 'r') {
                        pozicija2 = pozicija3 = pozicija;
                    }
                }
                if (pozicija2 >= 0) {
                    if (operand[pozicija2] == 'p' && operand[pozicija2-1] != 's') {
                        pozicija = pozicija3 = pozicija2;
                    }
                }

                if (pozicija3 >= 0) {
                    if (operand[pozicija3] == 's') {
                        pozicija = pozicija2 = pozicija3;
                    }
                }

            }
            else {
                cout << "Greska, fali operand na liniji" << brLinije << "\n";
                exit(-1);
            }
        }
        string reg;
        if (operand.length() >= 3)
            reg = operand.substr(pozicija, 2);
        else {
            cout << "Greska, fali registar na liniji" << brLinije << "\n";
            exit(-1);
        }
        if (reg != "r0" && reg != "r1" && reg != "r2" && reg != "r3" && reg != "r4" && reg != "r5" && reg != "r6" && reg != "r7" && reg != "pc" && reg != "sp") {
            cout << "Greska, koristite neispravan registar na liniji " << brLinije << "\n";
            exit(-1);
        }
        return (int)Asembler::kodoviReg.at(reg);
    }else {
        cout << "Greska, fali registar na liniji " << brLinije << "\n";
        exit(-1);
    }
}


void ParserLinije::odrediViseBiteRegistra(string operand, int brOperanda) {
    int pom;
    if((pom=operand.find_first_of("l")) != string::npos) {
        if(brOperanda == 1) visiBitiRegistraPrvi = false;
        if(brOperanda == 2) visiBitiRegistraDrugi = false;
    }
    if((pom=operand.find_first_of("h")) != string::npos) {
        if(brOperanda == 1) visiBitiRegistraPrvi = true;
        if(brOperanda == 2) visiBitiRegistraDrugi = true;
    }

}

void ParserLinije::parsiraj(string linija, int brLinije) {
    izraz = "";
    vrednosti = list<string>();
    vrednost = 0;
    simbol = "";
    simboli = list<string>();
    tipLinije = Prazna;

    int pozicija;
    //ako imam komentar, hocu da ga otklonim
    if((pozicija=linija.find_first_of("#")) != string::npos) {
        linija = linija.substr(0, pozicija);
    }

    //ako je labela
    if((pozicija=linija.find_first_of(":")) != string::npos) {
        int start = linija.find_first_not_of(" \t");
        labela = linija.substr(start, pozicija-start);
        if((labela.find_first_of(" \t")) != string::npos || labela.empty()) {
            cout << "Greska, neispravan naziv labele, linija " << brLinije << "\n";
            exit(-1);
        }
        //uzimam sve iza naziva labele
        linija = linija.substr(pozicija+1, linija.length() - pozicija - 1);
        if((pozicija = linija.find_first_not_of(" \t")) != string::npos) {
            linija = linija.substr(pozicija, linija.length() - pozicija);
        } else {
            linija = "";
        }
    }

    //ako je linija prazna
    if((pozicija = linija.find_first_not_of(" \t")) == string::npos) {
        tipLinije = Prazna;
        return;
    }
    
    //linija = linija.substr(pozicija, linija.length() - pozicija);
    int poz;
    if((poz = linija.find_first_not_of(" \t")) != string::npos) {
        linija = linija.substr(poz, linija.length() - poz);
    }

    //gledam da li pocinje tackom
    //gde se poslednji put pojavljuje .
    //ako se pojavljuje posle nulte pozicije->greska

    if((pozicija = linija.rfind("."))!=string::npos) {
        if(pozicija!=0) {
            cout << "Tacka na pogresnom mestu na liniji" << brLinije << "\n";
            exit(-1);
        } 
        linija = linija.substr(1, linija.length()-1); //uzmem ono iza tacke
        //ako pocinje tackom onda je ili sekcija ili direktiva

        //ako je sekcija
        //string pom1 = linija.substr(0, 7);
        int pom1 = 0;
        string s = "";
        
        if((pom1 = linija.find_first_of(" \t")) != string::npos) {
            s = linija.substr(0, pom1);
        }

        string pom2 = "section";
        if(s.compare(pom2) == 0) {
            //jeste sekcija
            tipLinije = Sekcija;
            //uzimam sve iza "section"
            linija = linija.substr(pom1, linija.length()-pom1);
            
            int poz;
            //brisem razmake
            if((poz=linija.find_first_not_of(" \t")) != string::npos) {
                linija = linija.substr(poz, linija.length()-poz);
                int pozicija;
                if((pozicija = linija.find_first_of(" \t")) == string::npos) {
                    sekcija = linija;
                    linija = "";
                } else {
                    sekcija = linija.substr(0, pozicija);
                    linija = linija.substr(pozicija, linija.length() - pozicija);
                }
                int kraj;
                if((kraj = linija.find_first_not_of(" \t")) != string::npos) {
                    sekcija = "";
                    cout << "Greska, neispravan tekst iza naziva sekcije na liniji " << brLinije << "\n";
                    exit(-1);
                }                
                //uzimam ono iza naziva sekcije
                //ukoliko ima nesto iza naziva sekcije, onda imam gresku
                
                //ako je sekcija, samo kupim njeno ime, i nema vise sta da se radi 
                return;
            }
        }

        //da li je direktiva
        //prolazim kroz niz direktiva i za onu koja jeste, odradim ovaj if
        for (int i = 0; i < 8; i++) {
            if (linija.rfind(Asembler::direktive[i], 0) == 0) {
                //pokupim naziv direktive, gore sam skinula tacku, pa o tome ne brinem
                direktiva = Asembler::direktive[i];

                //odsecem naziv direktive
                linija = linija.substr(direktiva.length(), linija.length() - direktiva.length());

                tipLinije = Direktiva;
                if (direktiva == "word" || direktiva == "byte") {
                    vector<string> pokupljeneVrednosti;
                    int poz;
                    //obrisem razmake
                    if ((poz = linija.find_first_not_of(" \t")) == string::npos) {
                        cout << "Greska, fale argumenti na liniji" << brLinije << "\n";
                        exit(-1);
                    }

                    linija = linija.substr(poz, linija.length() - poz);

                    //uzimam rec po rec

                    while (true) {
                        if ((poz = linija.find_first_of(" ,\t")) == string::npos) {
                            pokupljeneVrednosti.push_back(linija);
                        }
                        else {
                            pokupljeneVrednosti.push_back(linija.substr(0, poz));
                        }
                        if ((poz = linija.find_first_of(",")) == string::npos) {
                            for (string vr : pokupljeneVrednosti) {
                                if (vr[0] == '+') vr = vr.substr(1, vr.length() - 1);
                                vrednosti.push_back(vr);
                            }
                            return;
                        }
                        linija = linija.substr(poz + 1, linija.length() - poz - 1);
                        if ((poz = linija.find_first_not_of(" \t")) == string::npos) {
                            cout << "Greska, iza zareza nema parametara na liniji" << brLinije << "\n";
                            exit(-1);
                        }

                        linija = linija.substr(poz, linija.length() - poz);
                    }
                    for (string vr : pokupljeneVrednosti) {
                        if (vr[0] == '+') vr = vr.substr(1, vr.length() - 1);
                        vrednosti.push_back(vr);
                    }
                }

                if (direktiva == "skip") {
                    int poz;
                    if ((poz = linija.find_first_not_of(" \t")) == string::npos) {
                        cout << "Greska, fali parametar na liniji" << brLinije << "\n";
                        exit(-1);
                    }
                    else {
                        linija = linija.substr(poz, linija.length() - poz);
                    }

                    bool hex = false;
                    if (linija.length() > 1) {
                        if (linija[0] == '+') linija = linija.substr(1, linija.length() - 1);
                        if (linija[0] == '-') {
                            vrednost = -1;
                        }
                        if (linija[0] == '-' && linija[1] == '0' && linija[2] == 'x') {
                            vrednost = -1;
                        }
                        if (linija.substr(0, 2) == "0x") {
                            hex = true;
                            linija = linija.substr(2, linija.length() - 2);
                            vrednost = stoi(linija, 0, 16);
                        }
                    }

                    if (!hex) vrednost = stoi(linija);
                }

                if (direktiva == "equ") {
                    int poz1;
                    //obrisem razmake
                    if ((poz1 = linija.find_first_not_of(" \t")) == string::npos) {
                        cout << "Greska, fale argumenti na liniji" << brLinije << "\n";
                        exit(-1);
                    }


                    linija = linija.substr(poz1, linija.length() - poz1);
                    //uzimam rec po rec

                    if ((poz1 = linija.find_first_of(" ,\t")) == string::npos) {
                        cout << "Greska, nema naziva simbola na liniji" << brLinije << "\n";
                        exit(-1);
                    }
                    else {

                        simbol = linija.substr(0, poz1);
                    }


                    linija = linija.substr(poz1, linija.length() - poz1);
                    izraz = linija;

                }

                if (direktiva == "extern" || direktiva == "global") {
                    vector<string> pokupljeneVrednosti;
                    int poz;
                    //obrisem razmake
                    if ((poz = linija.find_first_not_of(" \t")) == string::npos) {
                        cout << "Greska, fale argumenti u redu na liniji" << brLinije << "\n";
                        exit(-1);
                    }

                    linija = linija.substr(poz, linija.length() - poz);

                    //uzimam rec po rec
                    while (!linija.empty()) {
                        if ((poz = linija.find_first_of(" ,\t")) == string::npos) {
                            pokupljeneVrednosti.push_back(linija);

                        }
                        else {
                            pokupljeneVrednosti.push_back(linija.substr(0, poz));
                        }

                        if ((poz = linija.find_first_of(",")) == string::npos) {

                            for (string vr : pokupljeneVrednosti) {
                                if (vr[0] == '+') vr = vr.substr(1, vr.length() - 1);
                                vrednosti.push_back(vr);
                            }
                            return;
                        }

                        linija = linija.substr(poz + 1, linija.length() - poz - 1);
                        if ((poz = linija.find_first_not_of(" \t")) == string::npos) {
                            cout << "Greska, iza zareza nema parametara na liniji" << brLinije << "\n";
                            exit(-1);
                        }

                        linija = linija.substr(poz, linija.length() - poz);
                    }

                    for (string vr : pokupljeneVrednosti) {
                        if (vr[0] == '+') vr = vr.substr(1, vr.length() - 1);
                        vrednosti.push_back(vr);
                    }
                }
                return;
            }
        }
        cout << "Greska, tacka na pogresnom mestu na liniji" << brLinije << "\n";
        exit(-1);
    }

    tipLinije=Instrukcija;
    instrukcija=linija;
        
    ParserLinije::pcRelPrvi = false;
    ParserLinije::pcRelDrugi = false;
    //velicinaParametara = 0;
    prviParam = "";
    drugiParam = "";
    ParserLinije::tipAdresiranjaPrviParam="";
    ParserLinije::kodAdresiranjaPrviParam=-1;
    ParserLinije::tipAdresiranjaDrugiParam="";
    ParserLinije::kodAdresiranjaDrugiParam=-1;
    ParserLinije::kodRegistraPrvi = -1;
    ParserLinije::kodRegistraDrugi = -1;
    nazivInstrukcije = "";

    velicinaInstrukcije = 1;//za prvi bajt-op kod
    jedanBajt = false;
    int pozicijaNova;
    bool slovo = false;
    //brisem razmake s pocetka
    if((pozicijaNova = instrukcija.find_first_not_of(" \t")) == string::npos) {
        cout << "Greska, na liniji" << brLinije << "\n";
        exit(-1);
    }

    instrukcija = instrukcija.substr(pozicijaNova, instrukcija.length()-pozicijaNova);

    if((pozicijaNova = instrukcija.find_first_of(" \t")) == string::npos) {
        nazivInstrukcije = instrukcija;
    } else nazivInstrukcije = instrukcija.substr(0, pozicijaNova);

    if(nazivInstrukcije[nazivInstrukcije.length()-1] == 'w') {
        //velicinaParametara = 2;
        jedanBajt = false;
        slovo = true;
        nazivInstrukcije = nazivInstrukcije.substr(0, nazivInstrukcije.length()-1);
    }

    if(nazivInstrukcije[nazivInstrukcije.length() - 1] == 'b' && nazivInstrukcije != "sub") {
        jedanBajt = true;
        slovo = true;
        //velicinaParametara = 1;
        nazivInstrukcije = nazivInstrukcije.substr(0, nazivInstrukcije.length() - 1);
    }
        
    brParam = -1;

    for(string param: Asembler::nulaParamOp) {
        if(param==nazivInstrukcije) {
            brParam = 0;
        }
    }
        
    for(string param: Asembler::jedanParamOp) {
        if(param==nazivInstrukcije) {
            brParam = 1;
        }
    }


    for(string param: Asembler::dvaParamOp) {
        if(param==nazivInstrukcije) {
            brParam = 2;
        }
    }
        
    if(brParam == -1) {
        cout << "Greska, neispravan naziv instrukcije na liniji " << brLinije << "\n";
        exit(-1);
    } 
    else {
        kodInstrukcije = Asembler::kodoviOp.at(nazivInstrukcije);
        
        if (brParam == 2) {
            if (!slovo)
                instrukcija = instrukcija.substr(nazivInstrukcije.length(), instrukcija.length() - nazivInstrukcije.length());
            else
                instrukcija = instrukcija.substr(nazivInstrukcije.length() + 1, instrukcija.length() - nazivInstrukcije.length() - 1);
            //brisem razmake
            if ((pozicijaNova = instrukcija.find_first_not_of(" \t")) != string::npos) {
                instrukcija = instrukcija.substr(pozicijaNova, instrukcija.length() - 1);
            }

            if ((pozicijaNova = instrukcija.find_first_of(" ,\t")) != string::npos) {
                prviParam = instrukcija.substr(0, pozicijaNova);
                instrukcija = instrukcija.substr(pozicijaNova, instrukcija.length() - pozicijaNova);
                if (prviParam[0] == '+') prviParam = prviParam.substr(1, prviParam.length() - 1);
                //Operand obradjenPrviParam = obradi(prviParam);

                tipAdresiranjaPrviParam = odrediTipAdresiranja(prviParam);
                if (tipAdresiranjaPrviParam == "imm") prviParam = prviParam.substr(1, prviParam.length() - 1); //skidam mu $

                if (tipAdresiranjaPrviParam == "imm" && nazivInstrukcije == "shr") {
                    cout << "Greska, neispravno adresiranje za dst operand na liniji " << brLinije << "\n";
                    exit(-1);
                }

                if (tipAdresiranjaPrviParam == "regdir" || tipAdresiranjaPrviParam == "regind") velicinaInstrukcije++; //nema Im/Di/Ad bajtova, samo OpDesc
                else if (jedanBajt && tipAdresiranjaPrviParam == "imm") {
                    velicinaInstrukcije += 2; //1 Im/Di/Ad bajt i OpDesc
                }
                else {
                    velicinaInstrukcije += 3;   //2 Im/Di/Ad bajta i OpDesc
                }

                if (pcRel == true) {
                    pcRelPrvi = true;
                    pcRel = false;
                }

                if (tipAdresiranjaPrviParam == "regdir" || tipAdresiranjaPrviParam == "regind" || tipAdresiranjaPrviParam == "regind16") {
                    kodRegistraPrvi = odrediKodRegistra(prviParam, brLinije);
                }

                if (tipAdresiranjaPrviParam == "regdir") {
                    odrediViseBiteRegistra(prviParam, 1);
                }

                kodAdresiranjaPrviParam = dohvatiKodAdresiranja(tipAdresiranjaPrviParam);

                if ((pozicijaNova = instrukcija.find_first_not_of(" ,\t")) != string::npos) {
                    instrukcija = instrukcija.substr(pozicijaNova, instrukcija.length() - pozicijaNova);
                }
                else {
                    cout << "Greska, neispravna instrukcija na liniji " << brLinije << "nema drugog parametra" << "\n";
                    exit(-1);
                }

                if ((pozicijaNova = instrukcija.find_first_not_of(" ,\t")) != string::npos) {
                    int pozN;
                    if ((pozN = instrukcija.find_first_of(" \t")) != string::npos) {
                        drugiParam = instrukcija.substr(pozicijaNova, pozN);
                        instrukcija = instrukcija.substr(pozN, instrukcija.length() - pozN);
                        if (drugiParam[0] == '+') drugiParam = drugiParam.substr(1, drugiParam.length() - 1);
                    }
                    else {
                        drugiParam = instrukcija;
                        instrukcija = "";
                    }

                    tipAdresiranjaDrugiParam = odrediTipAdresiranja(drugiParam);

                    if (tipAdresiranjaDrugiParam == "regdir" || tipAdresiranjaDrugiParam == "regind" || tipAdresiranjaDrugiParam == "regind16") {
                        kodRegistraDrugi = odrediKodRegistra(drugiParam, brLinije);
                    }

                    if (tipAdresiranjaDrugiParam == "regdir") {
                        odrediViseBiteRegistra(drugiParam, 2);
                    }

                    if (tipAdresiranjaDrugiParam == "imm") drugiParam = drugiParam.substr(1, drugiParam.length() - 1); //skidam mu $
                    if (tipAdresiranjaDrugiParam == "regdir" || tipAdresiranjaDrugiParam == "regind")
                        velicinaInstrukcije++;
                    else if (jedanBajt && tipAdresiranjaDrugiParam == "imm")
                        velicinaInstrukcije += 2;
                    else
                        velicinaInstrukcije += 3;

                    if (pcRel == true) {
                        pcRelDrugi = true;
                        pcRel = false;
                    }

                    if (tipAdresiranjaDrugiParam == "imm" && nazivInstrukcije != "shr") {
                        cout << "Greska, neispravan tip adresiranja na liniji " << brLinije << "\n";
                        exit(-1);
                    }

                    kodAdresiranjaDrugiParam = dohvatiKodAdresiranja(tipAdresiranjaDrugiParam);
                }
                cout << "INSTRUKCIJA POSLE DRUGOG\n" << instrukcija << "\n";

                if ((pozicijaNova = instrukcija.find_first_not_of(" \t")) != string::npos) {
                    prviParam = "";
                    drugiParam = "";
                    cout << "Greska, previse parametara na liniji" << brLinije << "\n";
                    exit(-1);
                }
            }
            else {
                cout << "Koristite dvoparametarsku instrukciju sa jednim parametrom\n";
                exit(-1);
            }
        }
        if(brParam == 1) {
            if(!slovo) instrukcija = instrukcija.substr(nazivInstrukcije.length(), instrukcija.length()-nazivInstrukcije.length());
            else instrukcija = instrukcija.substr(nazivInstrukcije.length()+1, instrukcija.length() - nazivInstrukcije.length() - 1);
     
            if((pozicijaNova = instrukcija.find_first_not_of(" \t")) != string::npos) {
                instrukcija = instrukcija.substr(pozicijaNova, instrukcija.length()-pozicijaNova);
            } else {
                cout << "Greska, fali parametar na liniji" << brLinije << "\n";
                exit(-1);
            }

            if((pozicijaNova = instrukcija.find_first_of(" ,\t")) == string::npos) {
                prviParam = instrukcija;
                instrukcija = "";
                if (prviParam[0] == '+') prviParam = prviParam.substr(1, prviParam.length() - 1);
            } else {
                prviParam = instrukcija.substr(0, pozicijaNova);
                if (prviParam[0] == '+') prviParam = prviParam.substr(1, prviParam.length() - 1);
                instrukcija = instrukcija.substr(pozicijaNova, instrukcija.length()-pozicijaNova);
            }
            tipAdresiranjaPrviParam = odrediTipAdresiranjaSkok(prviParam);

            if((jesteSkok(nazivInstrukcije) || nazivInstrukcije == "pop") && tipAdresiranjaPrviParam == "imm") {
                cout << "Neispravan tip adresiranja na liniji" << brLinije << "\n";
                exit(-1);
            }
            if(jesteSkok(nazivInstrukcije)) {
                if(tipAdresiranjaPrviParam != "imm") prviParam = prviParam.substr(1, prviParam.length()-1); //skidam mu *
                
                if(tipAdresiranjaPrviParam=="regdir" || tipAdresiranjaPrviParam == "regind" || tipAdresiranjaPrviParam == "regind16") {
                    kodRegistraPrvi = odrediKodRegistra(prviParam, brLinije);
                }

                if(pcRel == true) {
                    pcRelPrvi = true;
                    pcRel = false;  
                } 

                if(tipAdresiranjaPrviParam == "imm") {
                    cout << "Greska, neispravan tip adresiranja na liniji " << brLinije << "\n";
                    exit(-1);
                } 

                if(tipAdresiranjaPrviParam == "regdir" || tipAdresiranjaPrviParam == "regind") 
                    velicinaInstrukcije++;
                else if(jedanBajt && tipAdresiranjaPrviParam == "imm")
                    velicinaInstrukcije += 2;
                else
                    velicinaInstrukcije += 3;
                        
                kodAdresiranjaPrviParam = dohvatiKodAdresiranja(tipAdresiranjaPrviParam);
                    
            }
            else {
                //Operand obradjenPrviParam = obradi(prviParam);
                tipAdresiranjaPrviParam = odrediTipAdresiranja(prviParam); 
                        
                if(tipAdresiranjaPrviParam == "imm") prviParam = prviParam.substr(1, prviParam.length()-1); //skidam mu $
                if(tipAdresiranjaPrviParam=="regdir" || tipAdresiranjaPrviParam == "regind" || tipAdresiranjaPrviParam == "regind16") {
                    kodRegistraPrvi = odrediKodRegistra(prviParam, brLinije);
                }

                if(tipAdresiranjaPrviParam == "imm" && (nazivInstrukcije == "int" || nazivInstrukcije == "pop")) {
                    cout << "Greska, neispravan tip adresiranja na liniji " << brLinije << "\n";
                    exit(-1);
                }

                if(tipAdresiranjaPrviParam == "regdir" || tipAdresiranjaPrviParam == "regind") velicinaInstrukcije++;
                else {
                    if(jedanBajt) {
                        velicinaInstrukcije += 2;
                    } else {
                        velicinaInstrukcije += 3;
                    }
                }

                if(pcRel == true) {
                    pcRelPrvi = true;
                    pcRel = false;
                } 
                        
                if(tipAdresiranjaPrviParam == "imm") {
                    cout << "Greska, neispravan tip adresiranja na liniji " << brLinije << "\n";
                    exit(-1);
                } 

                kodAdresiranjaPrviParam = dohvatiKodAdresiranja(tipAdresiranjaPrviParam);
            }
            if((pozicijaNova = instrukcija.find_first_not_of(" \t")) != string::npos) {
                prviParam = "";
                cout << "Greska, na liniji" << brLinije << "\n";
                exit(-1);
            }
        }
    }
}