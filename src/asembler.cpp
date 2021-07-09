#pragma once
#include<iomanip>
#include<locale>
#include<iostream>
#include<string>
#include<regex>
#include <sstream>
#include "string.h"
#include<vector>
#include<math.h>
#include "asembler.h"

Asembler::Asembler(string ulaz, string izlaz) {
    this->ulaz.open(ulaz);
    this->izlazIme = izlaz;
    this->tabelaSimbola = TabelaSimbola();
    this->tabelaSekcija = TabelaSekcija();
    this->pocetnaInicijalizacija();
    this->jedanJediniProlaz();    
}

vector<string> Asembler::nulaParamOp = vector<string>();
vector<string> Asembler::jedanParamOp = vector<string>();
vector<string> Asembler::dvaParamOp = vector<string>();
vector<string> Asembler::skokovi = vector<string>();
unordered_map<string, int> Asembler::kodoviOp = unordered_map<string, int>();
unordered_map<string, int> Asembler::kodoviAdr = unordered_map<string, int>();
unordered_map<string, int> Asembler::kodoviReg = unordered_map<string, int>();

string Asembler::direktive[] = {"equ", "global", "extern", "byte", "word", "skip", "section", "end"};

void Asembler::pocetnaInicijalizacija() {
    nulaParamOp.push_back("halt");
    nulaParamOp.push_back("iret");
    nulaParamOp.push_back("ret");

    jedanParamOp.push_back("int");
    jedanParamOp.push_back("call");
    jedanParamOp.push_back("jmp");
    jedanParamOp.push_back("jeq");
    jedanParamOp.push_back("jne");
    jedanParamOp.push_back("jgt");
    jedanParamOp.push_back("push");
    jedanParamOp.push_back("pop");

    dvaParamOp.push_back("xchg");
    dvaParamOp.push_back("mov");
    dvaParamOp.push_back("add");
    dvaParamOp.push_back("sub");
    dvaParamOp.push_back("mul");
    dvaParamOp.push_back("div");
    dvaParamOp.push_back("cmp");
    dvaParamOp.push_back("not");
    dvaParamOp.push_back("and");
    dvaParamOp.push_back("or");
    dvaParamOp.push_back("xor");
    dvaParamOp.push_back("test");
    dvaParamOp.push_back("shl");
    dvaParamOp.push_back("shr");

    skokovi.push_back("call");
    skokovi.push_back("jmp");
    skokovi.push_back("jeq");
    skokovi.push_back("jne");
    skokovi.push_back("jgt");


    kodoviOp.insert(pair<string, int>("halt", 0x00));
    kodoviOp.insert(pair<string, int>("iret", 0x01));
    kodoviOp.insert(pair<string, int>("ret", 0x02));
    kodoviOp.insert(pair<string, int>("int", 0x03));
    kodoviOp.insert(pair<string, int>("call", 0x04));
    kodoviOp.insert(pair<string, int>("jmp", 0x05));
    kodoviOp.insert(pair<string, int>("jeq", 0x06));
    kodoviOp.insert(pair<string, int>("jne", 0x07));
    kodoviOp.insert(pair<string, int>("jgt", 0x08));
    kodoviOp.insert(pair<string, int>("push", 0x09));
    kodoviOp.insert(pair<string, int>("pop", 0x0A));
    kodoviOp.insert(pair<string, int>("xchg", 0x0B));
    kodoviOp.insert(pair<string, int>("mov", 0x0C));
    kodoviOp.insert(pair<string, int>("add", 0x0D));
    kodoviOp.insert(pair<string, int>("sub", 0x0E));
    kodoviOp.insert(pair<string, int>("mul", 0x0F));
    kodoviOp.insert(pair<string, int>("div", 0x10));
    kodoviOp.insert(pair<string, int>("cmp", 0x11));
    kodoviOp.insert(pair<string, int>("not", 0x12));
    kodoviOp.insert(pair<string, int>("and", 0x13));
    kodoviOp.insert(pair<string, int>("or", 0x14));
    kodoviOp.insert(pair<string, int>("xor", 0x15));
    kodoviOp.insert(pair<string, int>("test", 0x16));
    kodoviOp.insert(pair<string, int>("shl", 0x17));
    kodoviOp.insert(pair<string, int>("shr", 0x18));

    kodoviReg.insert(pair<string, int>("r0", 0x0));
    kodoviReg.insert(pair<string, int>("r1", 0x1));
    kodoviReg.insert(pair<string, int>("r2", 0x2));
    kodoviReg.insert(pair<string, int>("r3", 0x3));
    kodoviReg.insert(pair<string, int>("r4", 0x4));
    kodoviReg.insert(pair<string, int>("r5", 0x5));
    kodoviReg.insert(pair<string, int>("r6", 0x6));
    kodoviReg.insert(pair<string, int>("r7", 0x7));
    kodoviReg.insert(pair<string, int>("sp", 0x6));
    kodoviReg.insert(pair<string, int>("pc", 0x7));
    kodoviReg.insert(pair<string, int>("psw", 0xF));

    kodoviAdr.insert(pair<string, int>("imm", 0x0));
    kodoviAdr.insert(pair<string, int>("regdir", 0x1));
    kodoviAdr.insert(pair<string, int>("regind", 0x2));
    kodoviAdr.insert(pair<string, int>("regind16", 0x3));
    kodoviAdr.insert(pair<string, int>("mem", 0x4));
}

void Asembler::jedanJediniProlaz() {

    Sekcija* sekcijaIspis = nullptr;

    string linija;
    string direktiva;
    string labela;

    string pomLinija;
    int brLinije = 0;

    string izlazPom = "";
    
    trenutnaSekcija = -1; //jer je tek pocetak
    pomeraj = 0; //za svaku sekciju se resetuje
    imeTrenutneSekcije = "UND";
   

    if(!this->ulaz.is_open()) {
        cout << "Fajl nije otvoren!!!" << endl;
        this->izlaz.close();
        return;
    }

    while(!this->ulaz.eof()) {
        getline(this->ulaz, pomLinija, '\n');
        brLinije++; //prosto uvecavam za svaku novu liniju ulaznog fajla
                    //da znam ako se pojavi greska, gde se pojavila
    if(int(pomLinija[pomLinija.size() - 1]) == 13) pomLinija = pomLinija.substr(0, pomLinija.size() - 1);
        if(pomLinija.length() == 0) {
            continue;
        }

        ParserLinije isparsiranaLinija = ParserLinije(pomLinija, brLinije);

        if(isparsiranaLinija.dohvatiLabelu() != "") {
            cout << "LABELA" << isparsiranaLinija.dohvatiLabelu() << "\n"; 
            bool glob = false;
            int rb = -1;

            if(trenutnaSekcija < 0) { 
                cout << "Labela nije u sekciji, greska na liniji" << brLinije << "\n";
                return;
            }

            if(tabelaSimbola.postoji(isparsiranaLinija.dohvatiLabelu())) {
                Simbol* s = tabelaSimbola.pronadjiSimbol(isparsiranaLinija.dohvatiLabelu());
                
                if (s->definisan == true) {
                    if (s->simbolSekcija == false) {
                        cout << "Simbol " + s->ime << "je vec definisan\n";
                        return;
                    }
                }

                s->postaviDefinisan(pomeraj);
                s->sekcija = trenutnaSekcija;
                if(s->globalSimbol || s->externSimbol) glob = true;
                rb = s->redniBroj;
            } else {
                Simbol simbol = Simbol(isparsiranaLinija.dohvatiLabelu(), trenutnaSekcija, "l", tabelaSimbola.brojac++, pomeraj);
                simbol.postaviDefinisan(pomeraj);
                tabelaSimbola.dodajSimbol(simbol);
                rb = simbol.redniBroj;
            }

            for(Sekcija sekc: tabelaSekcija.dohvatiListu()) {
                for(RelokacioniZapis rz: sekc.dohvatiListuRelZapisa()) {
                    if(rz.dohvatiImeSimbola() == isparsiranaLinija.dohvatiLabelu()) {
                        if(glob) {
                            rz.postaviRedniBroj(rb);
                        }else {
                            rz.postaviRedniBroj(trenutnaSekcija);
                        }
                        
                    }
                } 
            }
        }

        if(isparsiranaLinija.dohvatiTipLinije() == ParserLinije::Sekcija) {
            //cout <<"SEKCIJA" <<"\n";
            //ovde treba da sacuvam velicinu prethodne sekcije
            
            if(trenutnaSekcija != -1) {
                tabelaSimbola.postaviVelicinuSekcije(trenutnaSekcija, pomeraj, imeTrenutneSekcije);
            } 
            
            imeTrenutneSekcije = isparsiranaLinija.dohvatiSekciju();
            trenutnaSekcija = tabelaSimbola.brojac;
            pomeraj = 0;
           
            tabelaSekcija.dodaj(isparsiranaLinija.dohvatiSekciju(), trenutnaSekcija);//pri svakom dodavanju ja pravim novu sekciju sa 1 tabelom rel zapisa
            Simbol s = Simbol(imeTrenutneSekcije, trenutnaSekcija, "l", tabelaSimbola.brojac++, 0);
            s.simbolSekcija=true;
            tabelaSimbola.dodajSimbol(s);
            sekcijaIspis = tabelaSekcija.dohvatiSekciju(trenutnaSekcija);
        }
        else if(isparsiranaLinija.dohvatiTipLinije() == ParserLinije::Direktiva) {
            direktiva = isparsiranaLinija.dohvatiDirektivu();
            
            if(direktiva == "end") {
               
                Simbol* s = tabelaSimbola.pronadjiSekciju(trenutnaSekcija);
                s->velicinaSekcije = pomeraj;
                sredi(tabelaSimbola, trenutnaSekcija, imeTrenutneSekcije, pomeraj);
                return;
            }

            if(trenutnaSekcija == -1 && direktiva != "global" && direktiva != "extern"  && direktiva != "equ") {
                cout << "Direktiva je van sekcije, greska na liniji" << brLinije << "\n";
                return;
            }
            
            if(direktiva == "skip") {
                //vrednost je jedan literal kastovan u int
                if(isparsiranaLinija.dohvatiVrednost() < 0) {
                    cout << "Uz direktivu skip je nedozvoljena negativna vrednost" << "\n";
                    return;
                }
                for(int i = 0; i < isparsiranaLinija.dohvatiVrednost(); i++) {
                    sekcijaIspis->postaviBajtoveZaIspis("00");
                    pomeraj += 1;
                    //on ce ovde da samo ispreskace te bajtove
                    //ne treba rel zapis jer nemam simbole        
                }

            }

            if(direktiva == "byte" || direktiva == "word") {
                int size = 1;
                if(direktiva == "word")
                    size = 2;

                for(string trenutni: isparsiranaLinija.dohvatiVrednosti()) {
                    bool jesteHex = false;

                    if(trenutni[0] == '-' && !isdigit(trenutni[1])) {
                        cout << "GRESKA, NE SME SE POJAVITI NEGATIVAN SIMBOL" << "\n";
                        return;
                    }

                    // Obradjuje ako je hex literal
                    if(trenutni.length() > 2) {
                        if((trenutni[0] == '-' || trenutni[0] == '+') && trenutni[1] == '0' && trenutni[2] == 'x') {
                            // negativan hex literal
                            sekcijaIspis->postaviBajtoveZaIspis(intToHex(0 - hexToDec(trenutni.substr(3, trenutni.length() - 3)), size));
                            pomeraj += size;
                            jesteHex = true;
                        }

                        if(trenutni[0] == '0' && trenutni[1] == 'x') {
                            // pozitivan hex literal
                            trenutni = trenutni.substr(2, trenutni.length()-2);
                            
                            if (trenutni.length() > 2 * size) {
                                trenutni = trenutni.substr(trenutni.length()-size*2, size*2);
                            }

                            if(trenutni.length() < size * 2) {
                                int uslov = size * 2 - trenutni.length();
                                for(int i = 0; i < uslov; i++) {
                                    trenutni = "0" + trenutni;
                                }
                            }  
                            sekcijaIspis->postaviBajtoveZaIspis((size == 1 ? trenutni : littleEndianDvaBajta(trenutni)));
                            pomeraj += size;
                            jesteHex = true;
                        }
                    }

                    if(!jesteHex) {
                        // Ako je decimalni literal
                        if((trenutni[0] == '+' || trenutni[0] == '-' ) && isdigit(trenutni[1]) || isdigit(trenutni[0])) {
                            int pom = atoi(trenutni.c_str()); //vratice 0 ako je neki simbol
                            sekcijaIspis->postaviBajtoveZaIspis(intToHex(pom, size));
                            pomeraj += 1 * size;
                        } 
                        // Ako je simbol
                        else {
                            Simbol* s = nullptr;
                            int redniBroj;

                            if(tabelaSimbola.postoji(trenutni)) {
                                s = tabelaSimbola.pronadjiSimbol(trenutni);
                                if(s->globalSimbol || s->externSimbol) 
                                    // Ako vec znamo da je global/extern postavi redni broj
                                    redniBroj = s->redniBroj;
                                else 
                                    // U suprotnom postavi sekciju (koja je -1 ako nije jos definisan)
                                    redniBroj = s->sekcija;
                            }
                            else {
                                Simbol novi = Simbol(trenutni, -1, "l", tabelaSimbola.brojac++, -1);
                                tabelaSimbola.dodajSimbol(novi);
                                s = tabelaSimbola.pronadjiSimbol(trenutni);
                                redniBroj = -1;
                            }

                            Pojavljivanje p = Pojavljivanje(trenutnaSekcija, 2*pomeraj, size);
                            s->listaPojavljivanja.push_back(p);
                            
                            RelokacioniZapis rz = RelokacioniZapis(trenutni, pomeraj, 0, redniBroj, size);
                            Sekcija *sekcija = tabelaSekcija.dohvatiSekciju(trenutnaSekcija);
                            sekcija->dodajRelZapis(rz);

                            for (int i = 0; i < size; i++)
                                sekcijaIspis->postaviBajtoveZaIspis("00"); 
                            pomeraj += 1 * size;              
                        }
                    }
                }
            }

            if(direktiva == "equ") {
                //ne povecavam pomeraj jer ovo necu ispisati u izlaz
                //samo cu da sacuvam u tabeli simbola
                string naziv = isparsiranaLinija.dohvatiSimbol();
                
                Simbol* s = tabelaSimbola.pronadjiSimbol(naziv);
                if(s == 0) {
                    Simbol sim = Simbol(naziv, trenutnaSekcija, "l", tabelaSimbola.brojac++, -1);
                    tabelaSimbola.dodajSimbol(sim);
                }
                Simbol* simbol = tabelaSimbola.pronadjiSimbol(naziv);
                simbol->postaviEqu(isparsiranaLinija.dohvatiIzraz());
            } 

            if(direktiva == "extern" || direktiva == "global") {
                    //dodam u tabelu simbola, stavim da je sekcija -1, da je ofset 0, da je vidljivost g
                for(string vrednost: isparsiranaLinija.dohvatiVrednosti()) {
                    Simbol *simbol = tabelaSimbola.pronadjiSimbol(vrednost);
                    if(simbol != 0) {
                        if(simbol->dohvatiVidljivost() != "g") {
                            //simbol->postaviGlobal();
                            if(direktiva=="global") simbol->globalSimbol=true;
                            else simbol->externSimbol = true;
                            simbol->vidljivost = "g";
                        }
                    } else {
                        Simbol simbol = Simbol(vrednost, -1, "g", tabelaSimbola.brojac++, -1);                            
                        if(direktiva=="global") simbol.globalSimbol=true;
                        else simbol.externSimbol = true;
                        tabelaSimbola.dodajSimbol(simbol);                        
                    }
                    list<Sekcija> listaSekc = tabelaSekcija.dohvatiListu();
                    for(int i = 0; i < listaSekc.size(); i++) {
                        for(Sekcija sekc: listaSekc) {
                            list<RelokacioniZapis> listaRelZap = sekc.dohvatiListuRelZapisa();
                            for(RelokacioniZapis rz: listaRelZap) {
                                if(rz.dohvatiImeSimbola() == vrednost) {
                                    rz.postaviRedniBroj(simbol->redniBroj);
                                }
                            } 
                        }                        
                    }

                }
            }
        } 
        else if(isparsiranaLinija.dohvatiTipLinije() == ParserLinije::Instrukcija) {
            if(trenutnaSekcija == -1) {
                cout << "Instrukcija je van sekcije, greska na liniji" << brLinije << "\n";
                return;
            } 

            int kod = (int) isparsiranaLinija.dohvatiKodInstrukcije();
            int velicina = isparsiranaLinija.dohvatiJedanBajt() ? 1: 2;

            kod = kod << 1;
            if(velicina == 2) kod += 1;
            kod = kod << 2;
            
            //u ovom trenutku ja imam formiran prvi bajt koji mogu da ispisem:
            sekcijaIspis->postaviBajtoveZaIspis(intToHexJedanBajt(kod));

            pomeraj += 1; //ispisala sam prvi bajt
            
            int velicinaCeleInstrukcije = isparsiranaLinija.dohvatiVelicinuInstrukcije();
            for(Parametar parametar: isparsiranaLinija.dohvatiParametre()) {
                int ret = ispisiParametar(parametar,
                                          velicina,
                                          isparsiranaLinija.dohvatiBrojParametara(),
                                          brLinije,
                                          velicinaCeleInstrukcije);
                if (ret < 0) 
                    return;
            }
        }
    }
}

int Asembler::ispisiParametar(Parametar param, int velicina, int brojParametara, int brLinije, int velicinaCeleInstrukcije) {
    int vrednostPCja = pomeraj + velicinaCeleInstrukcije;
    string adresiranje = param.tipAdresiranja;
    Sekcija* sekcijaIspis = tabelaSekcija.dohvatiSekciju(trenutnaSekcija);
    if(adresiranje == "imm") {
        int opDesc = 0;
        opDesc = opDesc << 3;
        opDesc += param.kodAdresiranja;
        opDesc = opDesc << 5;

        //ispisi sad drugi bajt
        
        sekcijaIspis->postaviBajtoveZaIspis(intToHexJedanBajt(opDesc));

        pomeraj += 1;
        string parametar = param.parametar;

        if(parametar[0] == '-' && !isdigit(parametar[1])) {
            cout << "GRESKA, NE SME SE POJAVITI NEGATIVAN SIMBOL" << "\n";
            return -1;
        }
        
        //ako mi je hex broj
        bool jesteHexParam = false;
        if(parametar.length() > 1) {
            if(parametar[0] == '-' && parametar[1] == '0' && parametar[2] == 'x') {
                jesteHexParam=true;
                parametar = parametar.substr(3, parametar.length()-3);
                sekcijaIspis->postaviBajtoveZaIspis(intToHex(0 - hexToDec(parametar), velicina));
            }
             
            if(parametar[0] == '0' && parametar[1] == 'x' || parametar[0] == '+' && parametar[1] == '0' && parametar[2] == 'x') {
                jesteHexParam = true;
                if(parametar[0] == '0')
                    parametar = parametar.substr(2, parametar.length() - 2);
                else 
                    parametar = parametar.substr(3, parametar.length() - 3);

                int dopuna = velicina * 2 - parametar.length();
                for (int i = 0; i < dopuna; i++)
                    parametar = '0' + parametar;

                parametar = parametar.substr(parametar.length() - velicina * 2, velicina * 2);
                   
                if(velicina == 2) 
                    sekcijaIspis->postaviBajtoveZaIspis(littleEndianDvaBajta(parametar));
                else 
                    sekcijaIspis->postaviBajtoveZaIspis(parametar);
            }
            pomeraj += velicina;
        }

        if(!jesteHexParam) {
            if(isdigit(parametar[0]) || ((parametar[0] == '+' || parametar[0] == '-') && isdigit(parametar[1]))) {
                sekcijaIspis->postaviBajtoveZaIspis(intToHex(stoi(parametar), velicina));
            } else {
                if(!tabelaSimbola.postoji(parametar)) {
                    Simbol simbol = Simbol(parametar, -1, "l", tabelaSimbola.brojac++, -1);
                    tabelaSimbola.dodajSimbol(simbol);
                }
                Simbol* s = tabelaSimbola.pronadjiSimbol(parametar);
                Pojavljivanje p = Pojavljivanje(trenutnaSekcija, 2*pomeraj, velicina);
                s->listaPojavljivanja.push_back(p);

                int redniBroj = -1;
                if(s->globalSimbol || s->externSimbol) redniBroj = s->redniBroj;
                else redniBroj = s->sekcija;

                //ovo je relokacioni zapis za prvi bajt posle koda naziva instrukcije()
                RelokacioniZapis rz = RelokacioniZapis(parametar, pomeraj, 0, redniBroj, velicina);
                Sekcija *sekcija = tabelaSekcija.dohvatiSekciju(trenutnaSekcija);
                sekcija->dodajRelZapis(rz);

                sekcijaIspis->postaviBajtoveZaIspis("00");
                if(velicina == 2) sekcijaIspis->postaviBajtoveZaIspis("00");
            }          
            pomeraj += velicina;
        }
    }

    if(adresiranje == "regdir") {
        int opDesc = 0;
        opDesc = opDesc << 3;
        opDesc += param.kodAdresiranja;
        opDesc = opDesc << 4;
        opDesc += param.kodRegistra;
        opDesc = opDesc << 1;
        int lh = 0;
        if(param.visiBitiRegistra) lh = 1;
        opDesc += lh;

        //ispisi i drugi operand
        sekcijaIspis->postaviBajtoveZaIspis(intToHexJedanBajt(opDesc));
        pomeraj += 1; 
        
    }

    if(adresiranje == "regind") {
        int opDesc = 0;
        opDesc = opDesc << 3;
        opDesc += param.kodAdresiranja;
        opDesc = opDesc << 4;
        opDesc += param.kodRegistra;
        opDesc = opDesc << 1;

        //ispisi i drugi bajt
        sekcijaIspis->postaviBajtoveZaIspis(intToHexJedanBajt(opDesc));
        pomeraj += 1; 
    }

    if(adresiranje == "regind16") {
        int opDesc = 0;
        opDesc = opDesc << 3;
        opDesc += param.kodAdresiranja;
        opDesc = opDesc << 4;
        opDesc += param.kodRegistra;
        opDesc = opDesc << 1;
        
        //ispisi i drugi bajt
        sekcijaIspis->postaviBajtoveZaIspis(intToHexJedanBajt(opDesc));
        pomeraj += 1; 

        string parametar = param.parametar;
        if(parametar[0] == '-' && !isdigit(parametar[1])) {
            cout << "GRESKA, NE SME SE POJAVITI NEGATIVAN SIMBOL" << "\n";
            return -1;
        }

        if (isdigit(parametar[0]) || parametar[0] == '-' || parametar[0] == '+' || (parametar[0] == '0' && parametar[1] == 'x')) {
            int poz;
            if ((poz = parametar.find_first_of("(")) == string::npos) {
                cout << "Greska, neispravna instrukcija na liniji, ne postoji otvorena zagrada" << brLinije << "\n";
                return -1;
            }
            parametar = parametar.substr(0, poz);
            bool hex = false;
            if (parametar.length() > 2) {
                if (parametar.substr(0, 2) == "0x" || parametar.substr(0, 3) == "+0x") {
                    hex = true;
                    if(parametar.substr(0, 3) == "+0x") 
                        parametar = parametar.substr(3, parametar.length() - 3);
                    else 
                        parametar = parametar.substr(2, parametar.length() - 2);

                    int dopuna = 2 - parametar.length();
                    for (int i = 0; i < dopuna; i++)
                        parametar = '0' + parametar;

                    parametar = parametar.substr(parametar.length() - 2, 2);

                    int br = hexToDec(parametar);
                    sekcijaIspis->postaviBajtoveZaIspis(littleEndianDvaBajta(intToHexDvaBajta(br)));
                    pomeraj += 2;
                } 
                if(parametar.substr(0, 3) == "-0x") {
                    hex = true;
                    parametar = parametar.substr(3, parametar.length()-3);
                    int dopuna = 2 - parametar.length();
                    for (int i = 0; i < dopuna; i++)
                        parametar = '0' + parametar;

                    parametar = parametar.substr(parametar.length() - 2, 2);
                    int br = 0 - hexToDec(parametar);
                    sekcijaIspis->postaviBajtoveZaIspis(littleEndianDvaBajta(intToHexDvaBajta(br)));
                    pomeraj += 2;
                }
            }
            
            if(!hex) {
                if(!isdigit(parametar[1]) && parametar[0] == '-') {
                    cout << "GRESKA, NEGATIVAN SIMBOL" << "\n";
                    return -1;
                }
                int prviBroj = stoi(parametar);
                sekcijaIspis->postaviBajtoveZaIspis(littleEndianDvaBajta(intToHexDvaBajta(prviBroj)));
                pomeraj += 2;
            }
        } 
        else { 
            if(param.pcRel) {
                //2B
                //ako mi je simbol
                string imeS;
                int pom;
                if((pom=parametar.find_first_of("(")) == string::npos) {
                    cout << "Greska, neispravna instrukcija na liniji" << brLinije << ", nema otvorene zagrade" << "\n";
                    return -1;
                }
                imeS = parametar.substr(0, pom);
                if(!tabelaSimbola.postoji(imeS)) {
                    Simbol novi = Simbol(imeS, -1, "l", tabelaSimbola.brojac++, -1);
                    tabelaSimbola.dodajSimbol(novi);
                }
                Simbol *s = tabelaSimbola.pronadjiSimbol(imeS);

                //U IZLAZNI FAJL CU DA UPISEM ILI -VELICINAOPERANDA ILI -VELICINAOPERANDA1-VELICINAOPERANDA2
                
                int velicinaOperanada = vrednostPCja - pomeraj;

                // TODO drugi operand moze biti druge velicine u odnosu na ovaj

                Pojavljivanje pojavljivanje = Pojavljivanje(trenutnaSekcija, 2*pomeraj, 2, pomeraj, velicinaOperanada, vrednostPCja);
                s->listaPojavljivanja.push_back(pojavljivanje);

                int rb;
                if(s->globalSimbol || s->externSimbol) rb = s->redniBroj;
                else rb=s->sekcija;

                int tip = 0;
                if(param.pcRel == true) tip = 1;

                RelokacioniZapis rz = RelokacioniZapis(imeS, pomeraj, tip, rb, 2);
                Sekcija *sekcija = tabelaSekcija.dohvatiSekciju(trenutnaSekcija);
                sekcija->dodajRelZapis(rz);
                        
                sekcijaIspis->postaviBajtoveZaIspis("0000");
                pomeraj += 2;
            } else {
                //ako mi je simbol
                int pom;
                if((pom=parametar.find_first_of("(")) == string::npos) {
                    cout << "Greska, neispravna instrukcija na liniji" << brLinije << ", nema otvorene zagrade" << "\n";
                    return -1;
                }

                string imeS = parametar.substr(0, pom);
                if(!tabelaSimbola.postoji(imeS)) {
                    Simbol novi = Simbol(imeS, -1, "l", tabelaSimbola.brojac++, -1);
                    tabelaSimbola.dodajSimbol(novi);
                }
                Simbol *s = tabelaSimbola.pronadjiSimbol(imeS);

                //U IZLAZNI FAJL CU DA UPISEM ILI -VELICINAOPERANDA ILI -VELICINAOPERANDA1-VELICINAOPERANDA2
                Pojavljivanje pojavljivanje = Pojavljivanje(trenutnaSekcija, 2*pomeraj, 2);
                s->listaPojavljivanja.push_back(pojavljivanje);

                int rb;
                if(s->globalSimbol || s->externSimbol) rb = s->redniBroj;
                else rb=s->sekcija;

                int tip = 0;
                if(param.pcRel == true) tip = 1; 

                RelokacioniZapis rz = RelokacioniZapis(imeS, pomeraj, tip, rb, 2);
                Sekcija *sekcija = tabelaSekcija.dohvatiSekciju(trenutnaSekcija);
                sekcija->dodajRelZapis(rz);
                        
                sekcijaIspis->postaviBajtoveZaIspis("0000");
                pomeraj += 2;
            }
        }
    }

    if(adresiranje == "mem") {
        int opDesc = 0;
        opDesc = opDesc << 3;
        opDesc += param.kodAdresiranja;
        opDesc = opDesc << 4;
        //opDesc += param.kodRegistra;
        opDesc = opDesc << 1;

        //ispisi i drugi operand
        sekcijaIspis->postaviBajtoveZaIspis(intToHexJedanBajt(opDesc));
        pomeraj += 1;

        //sad jos da ispisem 2 Im/Di/Ad bajta
        string parametar = param.parametar;
            if(parametar[0] == '-' && !isdigit(parametar[1])) {
            cout << "GRESKA, NE SME SE POJAVITI NEGATIVAN SIMBOL" << "\n";
            return -1;
        }

        bool hex = false;
        if(isdigit(parametar[0]) || (parametar[0] == '-' && isdigit(parametar[1]))) {                            
            sekcijaIspis->postaviBajtoveZaIspis(littleEndianDvaBajta(intToHexDvaBajta(stoi(parametar))));
            pomeraj += 2;
        } 
        else if(parametar.length() > 2) {
            if(parametar.substr(0, 3) == "-0x") {
                hex = true;
                parametar = parametar.substr(3, parametar.length()-3);

                int dopuna = 2 - parametar.length();
                for (int i = 0; i < dopuna; i++)
                    parametar = '0' + parametar;

                parametar = parametar.substr(parametar.length() - 2, 2);
                sekcijaIspis->postaviBajtoveZaIspis(littleEndianDvaBajta(intToHexDvaBajta((0 - hexToDec(parametar)))));
                pomeraj += 2;
            }
            if(parametar.substr(0, 2) == "0x" || parametar.substr(0, 3) == "+0x") {
                hex=true;
                if(parametar.substr(0, 2) == "0x")
                    parametar = parametar.substr(2, parametar.length() - 2);
                else 
                    parametar = parametar.substr(3, parametar.length() - 3);
                int dopuna = 2 - parametar.length();
                for (int i = 0; i < dopuna; i++)
                    parametar = '0' + parametar;

                parametar = parametar.substr(parametar.length() - 2, 2);

                sekcijaIspis->postaviBajtoveZaIspis(littleEndianDvaBajta(intToHexDvaBajta((hexToDec(parametar)))));
                pomeraj += 2; 
            } 
        }

        if(!hex && !isdigit(parametar[0]) && !isdigit(parametar[1])) {
            //simbol je
            if(!tabelaSimbola.postoji(parametar)) {
                Simbol s = Simbol(parametar, -1, "l", tabelaSimbola.brojac++, -1);
                tabelaSimbola.dodajSimbol(s);
            }

            Simbol *s = tabelaSimbola.pronadjiSimbol(parametar);

            int rb;
            if(s->globalSimbol || s->externSimbol) rb = s->redniBroj;
            else rb = s->sekcija;
            
            Pojavljivanje pojava = Pojavljivanje(trenutnaSekcija, 2*pomeraj, 2);
            s->listaPojavljivanja.push_back(pojava);
            RelokacioniZapis rz = RelokacioniZapis(parametar, pomeraj, 0, rb, 2);
            Sekcija *sekcija = tabelaSekcija.dohvatiSekciju(trenutnaSekcija);
            sekcija->dodajRelZapis(rz); 

            sekcijaIspis->postaviBajtoveZaIspis("0000");
            pomeraj += 2;
        }
    }

    return 0;
}

void Asembler::sredi(TabelaSimbola tabelaSimbola, int trenutnaSekcija, string imeTrenutneSekcije, int pomeraj) {
    this->izlaz.open(this->izlazIme);

    tabelaSimbola.postaviVelicinuSekcije(trenutnaSekcija, pomeraj, imeTrenutneSekcije);
    string ime = tabelaSimbola.dohvatiNazivSekcije(trenutnaSekcija);
    Simbol *s = tabelaSimbola.pronadjiSimbol(ime);

    // Proveri da li ima nedefinisanih
    for (Simbol s : tabelaSimbola.dohvatiSveSimbole()) {
        if (s.pomeraj == -1 && s.externSimbol == false && s.equSimbol == false) {
            cout << "IME" << s.dohvatiIme();
            cout << "Nedefinisan simbol " << s.ime << " u tabeli simbola M" << s.dohvatiIme() << "\n";
            return;
        }
    }

    // Razresi equ
    while (true) {
        bool imaNerazresenih = false;
        bool imaRazresenih = false;
        for (Simbol& sim : tabelaSimbola.dohvatiSveSimbole()) {
            if (sim.equSimbol) {
                list<string>* elementi = &sim.equLista;
                list<string> razreseni = list<string>();

                for (string elem : *elementi) {
                    // tODO test length
                    if (isdigit(elem[1]) || (elem[1] == '0' && elem[2] == 'x')) {
                        int broj;
                        bool hex = false;

                        if (elem.length() > 2) {
                            if (elem[1] == '0' && elem[2] == 'x') {
                                hex = true;
                                broj = hexToDec(elem.substr(3, elem.length() - 3));
                            }
                        }
                        if (!hex) {
                            broj = stoi(elem.substr(1, elem.length() - 1));
                        }
                        if (elem[0] == '+') {
                            sim.equVrednost += broj;
                            razreseni.push_back(elem);
                        }
                        else {
                            sim.equVrednost -= broj;
                            razreseni.push_back(elem);
                        }
                    }
                    else {
                        if (elem.length() > 1) {
                            Simbol* simbolN = tabelaSimbola.pronadjiSimbol(elem.substr(1, elem.length() - 1));
                            if (simbolN == nullptr) {
                                cout << "Simbol od koga zavisi equ ne postoji u tabeli simbola T" << elem.substr(1, elem.length()-1)<<"\n";
                                return;
                            }
                            if (!simbolN->equSimbol) {
                                if(elem[0] == '+')
                                    sim.equVrednost += simbolN->pomeraj;
                                else
                                    sim.equVrednost -= simbolN->pomeraj;
                                razreseni.push_back(elem);
                            }
                            else if (simbolN->equLista.empty()) {
                                if(elem[0] == '+')
                                    sim.equVrednost += simbolN->equVrednost;
                                else 
                                    sim.equVrednost -= simbolN->equVrednost;
                                razreseni.push_back(elem);
                            }
                        }
                    }
                }

                bool bioRazresen = false;
                if (elementi->empty()) bioRazresen = true;

                for (string razresen : razreseni) {
                    elementi->remove(razresen);
                }

                if (!elementi->empty())
                    imaNerazresenih = true;
                else if(!bioRazresen)
                    imaRazresenih = true;
            }
        }

        if (!imaNerazresenih) {
            break;
        }

        if (!imaRazresenih) {
            cout << "Greska, ne moze se razresiti equ simbol\n";
            return;
        }
    }

    //za relokacione zapise, pomeraj je sredjen upisom svakog novog global/local tj definisanjem svakog simbola

    // sredi pojavljivanja
    for(Simbol s: tabelaSimbola.dohvatiSveSimbole()) {
        if(!s.equSimbol) {
            list<Pojavljivanje>* pojavljivanja = &s.listaPojavljivanja;
            for(Pojavljivanje& p: *pojavljivanja) {
                string ispis;
                if(p.velicina == 2 && s.vidljivost=="l") {
                    int pom;
                    if(p.sekcija == s.sekcija) {
                        pom = s.pomeraj - p.vrednostPCja;
                    } else {
                        pom = s.pomeraj - p.velicinaOperanada;
                    }
                    ispis = littleEndianDvaBajta(intToHexDvaBajta(pom));                   
                } else if(p.velicina == 1 && s.vidljivost=="l") {
                     int pom;
                    if(p.sekcija == s.sekcija) {
                        pom = s.pomeraj - p.vrednostPCja;
                    } else {
                        pom = s.pomeraj - p.velicinaOperanada;
                    }
                    ispis = intToHexJedanBajt(pom);
                } else if(p.velicina == 2 && s.vidljivost=="g") {
                    if(p.pomeraj != 0) {
                        int pom;
                        if(p.sekcija == s.sekcija) {
                            pom = s.pomeraj - p.vrednostPCja;
                        } else {
                            pom = - p.velicinaOperanada;
                        }
                        ispis += littleEndianDvaBajta(intToHexDvaBajta(pom));
                    } else {
                        ispis = "00";
                        ispis += "00";
                    }
                } else if(p.velicina == 1 && s.vidljivost=="g") {
                    if(p.pomeraj != 0) {
                             int pom;
                         if(p.sekcija == s.sekcija) {
                             pom = s.pomeraj - p.pomeraj;
                         } else {
                             pom = - p.velicinaOperanada;
                         }
                         ispis += intToHexJedanBajt(pom);
                         } else {
                            ispis = "00";                            
                         }
                }
              

                Sekcija* sekcijaPojave = tabelaSekcija.dohvatiSekciju(p.sekcija);
                string bajtoviUSekciji = sekcijaPojave->dohvatiBajtoveZaIspis();
                bajtoviUSekciji.replace(p.adresa, ispis.length(), ispis);    
                sekcijaPojave->zameniBajtove(bajtoviUSekciji);
            }
        } else {
            list<Pojavljivanje>* pojavljivanja = &s.listaPojavljivanja;
            for(Pojavljivanje &p: *pojavljivanja) {
                string ispis;
                if (p.velicinaOperanada != 0) {
                    cout << "Equ izraz se ne moze koristiti za PC-relativno adresiranje\n";
                    return;
                }
                if(p.velicina == 2) {
                    ispis += littleEndianDvaBajta(intToHexDvaBajta(s.equVrednost));
                } else if(p.velicina == 1){
                    ispis = intToHexJedanBajt(s.equVrednost);
                }
                Sekcija* sekcijaPojave = tabelaSekcija.dohvatiSekciju(p.sekcija);
                string bajtoviUSekciji = sekcijaPojave->dohvatiBajtoveZaIspis();
                bajtoviUSekciji.replace(p.adresa, ispis.length(), ispis);
                sekcijaPojave->zameniBajtove(bajtoviUSekciji);               
            }
        }
    }

    izlaz << setw(30) << "TABELA SIMBOLA\n";
    ispisTabeleSimbola(tabelaSimbola);
    izlaz << "\n";
    izlaz << setw(30) << "TABELERELOKACIONIH ZAPISA\n";
    ispisTabelaRelokacija(tabelaSekcija);
    izlaz << setw(30) << "PREDMETNI PROGRAMI\n";
    ispisKoda();
    izlaz << "\n";
    this->izlaz.close();
       
}

void Asembler::ispisTabeleSimbola(TabelaSimbola tabelaSimbola) {
    izlaz << setw(10) << "IME " << setw(10) << "SEKCIJA " << setw(10) << "POMERAJ " << setw(10) << "VIDLJIVOST " << setw(10) << "REDNI BROJ " << setw(10) << "VELICINA SEKCIJE\n";
    cout <<  "IME | SEKCIJA | POMERAJ | VIDLJIVOST | REDNI BROJ | VELICINA SEKCIJE\n";
    list<Simbol> listaSimbola = tabelaSimbola.dohvatiSveSimbole();
    for(Simbol s: listaSimbola) {
        stringstream ss;
        ss << std::hex << s.pomeraj;
        string noviPom(ss.str());
        if(s.simbolSekcija) {
            izlaz << setw(10) << s.ime << setw(10) << s.sekcija << setw(10) << noviPom << setw(10) << s.vidljivost << setw(10) << s.redniBroj << setw(10) << s.velicinaSekcije;
            izlaz << "\n";
            cout << "\n" << s.ime << setw(10) << s.sekcija << setw(10) << noviPom << setw(10) << s.vidljivost << setw(10) << s.redniBroj << setw(10) << s.velicinaSekcije;
        }
    }
    for(Simbol s: listaSimbola) {
        stringstream ss;
        ss << std::hex << s.pomeraj;
        string noviPom(ss.str());
        if(!s.simbolSekcija && !s.equSimbol) {
            izlaz << setw(10) << s.ime << setw(10) << s.sekcija << setw(10) << noviPom << setw(10) << s.vidljivost << setw(10) << s.redniBroj;
            izlaz << "\n";
            cout << "\n" << s.ime << setw(10) <<  s.sekcija << setw(10) << noviPom << setw(10) << s.vidljivost << setw(10) << s.redniBroj;
        }
    }
}

void Asembler::ispisTabelaRelokacija(TabelaSekcija tabelaSekcija) {
    list<Sekcija> sekcije = tabelaSekcija.dohvatiListu();
    bool imaZapisa;
    for(Sekcija s: sekcije) {
        imaZapisa = false;
        izlaz << setw(10) << "Relokacioni zapisi za sekciju " << s.dohvatiNaziv() << "\n";
        izlaz << setw(10) <<"IME" << setw(10) << "POMERAJ" << setw(10) << "TIP " << setw(10) <<"REDNI BROJ\n";
        cout<< "\n" << "Relokacioni zapisi za sekciju " << s.dohvatiNaziv() << "\n";
        cout << "IME | POMERAJ | TIP | REDNI BROJ";
        
        list<RelokacioniZapis> rel = s.dohvatiListuRelZapisa();
        
        for(RelokacioniZapis r: rel) {
            Simbol *simbol = tabelaSimbola.pronadjiSimbol(r.dohvatiImeSimbola());

            if(simbol->globalSimbol || simbol->externSimbol) {
                r.postaviRedniBroj(simbol->redniBroj);
            } else {
                r.postaviRedniBroj(simbol->sekcija);
            }
            string tip = "";
            if(r.dohvatiTip() == 0) {
                int pom = r.dohvatiPomeraj();
                if (r.dohvatiVelicinu() == 1) {
                    tip = "R_8";
                }
                else {
                    tip = "R_16";
                }
            } else {
                if (r.dohvatiVelicinu() == 2) {
                    tip = "R_PC16";
                }
                else {
                    tip = "R_PC8";
                }
            }
            if(!simbol->equSimbol && !((tip == "R_PC16" || tip == "R_PC8") && s.dohvatiBroj() == simbol->sekcija)) {
                stringstream ss;
                ss << std::hex << r.dohvatiPomeraj();
                string rezultat(ss.str());
                int dopuna = 8 - rezultat.length();
                for (int i = 0; i < dopuna; i++) rezultat = "0" + rezultat;
                imaZapisa = true;
                izlaz << setw(10) << r.dohvatiImeSimbola() << setw(10) << rezultat << setw(10) << tip << setw(10) << r.dohvatiRedniBroj() << "\n";
                cout << r.dohvatiImeSimbola() << setw(10) << rezultat << setw(10) << tip << setw(10) << r.dohvatiRedniBroj();
            }
        }

        if(!imaZapisa) {
            izlaz << "----- NEMA RELOKACIONIH ZAPISA ZA OVU SEKCIJU ----\n";
        }

        cout << "\n\n";
        izlaz << "\n\n";

    }
}

void Asembler::ispisKoda() {

    cout << "PREDMETNI PROGRAMI PO SEKCIJAMA\n" << "\n";

    for (Sekcija sekcija : tabelaSekcija.dohvatiListu()) {
        izlaz << "*** Sekcija " + sekcija.dohvatiNaziv() + " ***\n";
        
        string ispis = "";
        int granica = 28;
        string bajtovi = sekcija.dohvatiBajtoveZaIspis();
        for (int i = 0; i < bajtovi.length(); i++, granica--) {
            ispis += bajtovi[i];
            if (i % 2)
                if (granica < 0) {
                    ispis += "\n";
                    granica = 28;
                }
                else {
                    ispis += " ";
                }
        }

        izlaz << ispis + "\n";
        bajtovi = "";
        cout << "\n" << ispis;
    }
    izlaz << "\n\n";
    
}

int Asembler::hexToDec(string hexBroj) {
    int broj = 0;
    int pom = stoi(hexBroj, nullptr, 16);
    return pom;
}

string Asembler::intToHex(int broj, int size) {
    switch (size) {
        case 1: return intToHexJedanBajt(broj);
        case 2: return littleEndianDvaBajta(intToHexDvaBajta(broj));
    }
    return "ZZ";
}

string Asembler::intToHexJedanBajt(int broj) {
    bool negativan = false;
    if(broj < 0) negativan = true;
    broj = broj % 256;
	stringstream ss;
	ss << std::hex << broj;
	string rezultat(ss.str());

	int dopuna = 2 - rezultat.length();
	if (dopuna > 0)
		for (int i = 0; i < dopuna; i++) {
            rezultat = "0" +rezultat;
        }
        if(negativan == true) {
            if(rezultat.length() > 2) {
                rezultat = rezultat.substr(rezultat.length()-2, 2);
            }
            else {
                for (int i = 0; i < 2-rezultat.length(); i++) {
                    rezultat = "f" +rezultat;
                }
            }
        }

	return rezultat;
}

string Asembler::intToHexDvaBajta(int broj) {
    bool negativan = false;
    if(broj < 0) negativan = true;

    broj = broj % 65536;

	stringstream ss;
	ss << std::hex << broj;
	string rezultat(ss.str());

	int dopuna = 4 - rezultat.length();
	if (dopuna > 0)
		for (int i = 0; i < dopuna; i++) {
            rezultat = "0" +rezultat;
        }
    if(negativan == true) {
        if(rezultat.length() > 4) {
            rezultat = rezultat.substr(rezultat.length()-4, 4);
        }
        else {
            for (int i = 0; i < 4-rezultat.length(); i++) {
                rezultat = "f" +rezultat;
            }
        }
    }
	return rezultat;
} 

string Asembler::littleEndianDvaBajta(string bajtovi) {
    string novi = "";

    if(bajtovi.length() == 4) {
        novi.push_back(bajtovi[2]);
        novi += bajtovi[3];
        novi += bajtovi[0];
        novi += bajtovi[1];
    }
    return novi;
}
