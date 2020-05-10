#include <iostream>
#include <stdexcept>

template <typename tip>
class Lista {
    
    public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual tip &trenutni() = 0;
    virtual tip trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const tip &el) = 0;
    virtual void dodajIza(const tip &el) = 0;
    virtual tip operator [] (int i) const = 0;
    virtual tip &operator [] (int i) = 0;
};

template <typename tip>
class NizLista : public Lista<tip> {
    int duzina, kapacitet, tekuci;
    tip** p;
    
    void ObrisiNiz() {
        if (p) {
            for (int i=0; i<duzina; i++) if (p[i]!=nullptr) delete p[i];
            delete[] p;
        }
        p=nullptr;
    }
    
    void AlocirajMemoriju() {
        try {
            p=new tip*[kapacitet]{};
        } 
        catch(...) {
            throw "Nedovoljno memorije";
        }
    }
    
    public:
    NizLista() {
        duzina=0; tekuci=-1; kapacitet=50;
        p=nullptr;
    }
    
    ~NizLista() {
        ObrisiNiz();
        duzina=0; tekuci=-1;
    }
    
    
    NizLista(const NizLista<tip> &l) {
        if (l.duzina) { 
            tekuci=l.tekuci;
            duzina=l.duzina;
            kapacitet=l.kapacitet;
            try {
                AlocirajMemoriju();
                for (int i=0; i<duzina; i++) {
                    p[i]=new tip(*(l.p[i]));
                }
            }
            catch(...) {
                ObrisiNiz();
                duzina=0; tekuci=-1; kapacitet=50;
            }
        }
        else {
            duzina=0; tekuci=-1; kapacitet=50;
            p=nullptr;
        }
    }
    
    NizLista(NizLista<tip> &&l) {
        tekuci=l.tekuci;
        duzina=l.duzina;
        kapacitet=l.kapacitet;
        p=l.p;
        l.p=nullptr;
    }
    
    
    NizLista &operator =(const NizLista<tip> &l) {
        
        if (l.p!=p) {
            ObrisiNiz();
            tekuci=l.tekuci;
            duzina=l.duzina;
            kapacitet=l.kapacitet;
            try {
                AlocirajMemoriju();
                for (int i=0; i<duzina; i++) {
                    p[i]=new tip(*(l.p[i]));
                }
            }
            catch(...) {
                ObrisiNiz();
                duzina=0; tekuci=-1; kapacitet=50;
            }
        }
        return *this;
    }
    
    NizLista &operator =(NizLista<tip> &&l) {
        if (l.p!=p) {
            tekuci=0;
            duzina=l.duzina;
            kapacitet=l.kapacitet;
            std::swap(p, l.p);
        }
        return *this;
    }

    int brojElemenata() const override {
        return duzina;
    }
    
    tip &trenutni() override {
        if (tekuci==-1) throw std::logic_error("Prazna lista");
        return *p[tekuci];
    }
    
    tip trenutni() const override {
        if (tekuci==-1) throw std::logic_error("Prazna lista");
        return *p[tekuci];
    }
    
    bool prethodni() override {
        if (tekuci==-1) throw std::logic_error("Prazna lista");
        if (tekuci==0) return false;
        tekuci--;
        return true;
    }
    
    bool sljedeci() override {
        if (tekuci==-1) throw std::logic_error("Prazna lista");
        if (tekuci==duzina-1) return false;
        tekuci++;
        return true;
    }
    
    void pocetak() override { 
        if (tekuci==-1) throw std::logic_error("Prazna lista");
        tekuci=0;
    }
    
    void kraj() override {
        if (tekuci==-1) throw std::logic_error("Prazna lista");
        tekuci=duzina-1;
    }
    
    void obrisi() override {
        if (tekuci==-1) throw std::logic_error("Prazna lista");
        if (duzina==1) { ObrisiNiz(); duzina=0; kapacitet=50; tekuci=-1; p=nullptr; }
        else {
            duzina--;
            delete p[tekuci];
            for (int i=tekuci; i<duzina; i++) {
                p[i]=p[i+1];
            }
            p[duzina]=nullptr;
        }
        if (tekuci==duzina) tekuci--;
    }
    
    void dodajIspred(const tip &el) override {
        duzina++;
        
        if (tekuci==-1) {
            AlocirajMemoriju();
            tekuci=0;
            
            p[0]=new tip(el);
            return;
        }
        
        if (duzina<=kapacitet) {
            for (int i=duzina-1; i>tekuci && i>0; i--) p[i]=p[i-1]; 
            p[tekuci]=nullptr; 
            p[tekuci]=new tip(el);
            tekuci++;
            return;
        }
        
        kapacitet*=2;
        tip** novi=new tip*[kapacitet]{};
        
        for (int i=0; i<tekuci; i++) {
            novi[i]=p[i];
        }
        novi[tekuci]=new tip(el);
        tekuci++;
        for (int i=tekuci; i<duzina; i++) {
            novi[i]=p[i-1];
        }
        
        delete[] p;
        p=novi;
        novi=nullptr;
    }
    
    void dodajIza(const tip &el) override {
        duzina++;
        
        if (tekuci==-1) {
            if (p==nullptr) AlocirajMemoriju();
            tekuci=0;
            p[0]=new tip(el);
            return;
        }
        
        if (duzina<=kapacitet) {
            for (int i=duzina-1; i>tekuci && i>0; i--) p[i]=p[i-1];
           
            p[tekuci+1]=new tip(el);
            return;
        }
        
        kapacitet*=2;
        tip** novi=new tip*[kapacitet]{};
        
        for (int i=0; i<=tekuci; i++) {
            novi[i]=p[i];
        }
        novi[tekuci+1]=new tip(el);
        for (int i=tekuci+2; i<duzina; i++) {
            novi[i]=p[i-1];
        }
        
        delete[] p;
        p=novi;
        novi=nullptr;
    }
    
    tip operator [] (int i) const override {
        if (i<0 || i>=duzina) throw std::domain_error("Neispravan argument");
        return *p[i];
    }
    
    tip &operator [] (int i) override {
        if (i<0 || i>=duzina) throw std::domain_error("Neispravan argument");
        return *p[i];
    }
    
};

template <typename tip>
class JednostrukaLista : public Lista<tip> {
    
    struct Cvor {
        tip element;
        Cvor* sljedeci;
    };
    Cvor* prvi;
    Cvor* tekuci;
    int duzina;
    
    void ObrisiListu() {
        if (prvi!=nullptr) {
            tekuci=nullptr;
            while (duzina) obrisi();
        }
    }
    
    public:
    
    JednostrukaLista() {
        prvi=nullptr;
        tekuci=nullptr;
        duzina=0;
    }
    
    ~JednostrukaLista() {
        ObrisiListu();
    }
    
    JednostrukaLista(const JednostrukaLista<tip> &l) {
        if (l.duzina) {
            duzina=l.duzina;
            
            Cvor* p1(l.prvi), *prethodni(nullptr), *nova(nullptr);
            prvi=nullptr; tekuci=nullptr;
            
            while (p1!=nullptr) {
                
                nova=new Cvor;
                nova->element=p1->element;
                nova->sljedeci=nullptr;
                
                if (!prvi) prvi=nova;
                else prethodni->sljedeci=nova;
                
                prethodni=nova;
                
                if (p1==l.tekuci) tekuci=prethodni; 
                
                p1=p1->sljedeci;
            }
        }
        else {
            tekuci=nullptr; prvi=nullptr; duzina=0;
        }
    }
    
    JednostrukaLista(JednostrukaLista<tip> &&l) {
        
        duzina=l.duzina;
        prvi=l.prvi;
        tekuci=l.tekuci;
        l.prvi=nullptr; l.tekuci=nullptr; l.duzina=0;
    }
    
    
    JednostrukaLista &operator =(const JednostrukaLista<tip> &l) {
        
        if (l.prvi!=prvi) {
            ObrisiListu();
            
            JednostrukaLista temp(l);
            *this=std::move(temp);
            
        }
        return *this;
    }
    
    JednostrukaLista &operator =(JednostrukaLista<tip> &&l) {
        if (l.prvi!=prvi) {
            tekuci=l.tekuci; l.tekuci=nullptr;
            duzina=l.duzina;
            prvi=l.prvi; l.prvi=nullptr;                                                 
        }
        return *this;
    }
    
    int brojElemenata() const override {
        return duzina;
    }
    
    tip trenutni() const override {
        if (duzina==0) throw std::logic_error("Prazna lista");
        if (tekuci==nullptr) return prvi->element;
        return (tekuci->sljedeci)->element;
    }
    
    tip &trenutni() override {
        if (duzina==0) throw std::logic_error("Prazna lista");
        if (tekuci==nullptr) return prvi->element;
        return (tekuci->sljedeci)->element;
    }
    
    bool prethodni() override {
        if (duzina==0) throw std::logic_error("Prazna lista");
        if (tekuci==nullptr) return false;
        
        if (tekuci==prvi) { tekuci=nullptr; return true; }
        
        Cvor* p(prvi);
        while (p->sljedeci!=tekuci) p=p->sljedeci;
        
        tekuci=p;
        return true;
    }
    
    bool sljedeci() override {
        if (prvi==nullptr) throw std::logic_error("Prazna lista");
        if (tekuci==nullptr) { 
            if (prvi->sljedeci!=nullptr) { tekuci=prvi; return true; }
            else return false;
        }
        if ((tekuci->sljedeci)->sljedeci==nullptr) return false;
        
        tekuci=tekuci->sljedeci;
        return true;
    }
    
    void pocetak() override {
        if (prvi==nullptr) throw std::logic_error("Prazna lista");
        tekuci=nullptr;
    }
    
    void kraj() override {
        if (prvi==nullptr) throw std::logic_error("Prazna lista");
        if (prvi->sljedeci==nullptr) { tekuci=nullptr; return; }
        if (tekuci==nullptr) tekuci=prvi;
        Cvor* temp(tekuci);
        temp=temp->sljedeci;
        while (temp->sljedeci!=nullptr) { tekuci=tekuci->sljedeci; temp=temp->sljedeci; }
    }
    
    void obrisi() override {
        if (prvi==nullptr) throw std::logic_error("Prazna lista");
        
        duzina--;
        
        if (tekuci==nullptr) {
            tekuci=prvi;
            prvi=prvi->sljedeci;
            delete tekuci;
            tekuci=nullptr;
            return;
        }
        
        Cvor *p1(tekuci);
        
        p1=p1->sljedeci;
        
        tekuci->sljedeci=p1->sljedeci;
        
        delete p1;
        
        if (tekuci->sljedeci==nullptr) {
            if (prvi->sljedeci==nullptr) tekuci=nullptr;
            else {
               Cvor* prethodni(prvi);
                while (prethodni->sljedeci!=tekuci) prethodni=prethodni->sljedeci;
                tekuci=prethodni; 
            }
        }
    }
    
    void dodajIspred(const tip &el) override {
        duzina++;
        if (duzina==1) {
            prvi=new Cvor;
            prvi->element=el;
            prvi->sljedeci=nullptr;
            
            tekuci=nullptr;
        }
        else if (tekuci==nullptr) {
            tekuci=new Cvor;
            tekuci->element=el;
            tekuci->sljedeci=prvi;
            prvi=tekuci;
            tekuci=prvi;
        }
        else {
            Cvor* p(tekuci);
            tekuci=new Cvor;
            tekuci->element=el;
            tekuci->sljedeci=p->sljedeci;
            p->sljedeci=tekuci;
        }
        
        
    }
    
    void dodajIza(const tip &el) override {
        
        if (duzina==0) {
            
            prvi=new Cvor;
            prvi->element=el;
            prvi->sljedeci=nullptr;
            
            tekuci=nullptr;
            duzina++;
        }
        else if (tekuci==nullptr) {
            tekuci=prvi;
            dodajIspred(el);
            tekuci=nullptr;
        }
        else  {
            Cvor *temp(tekuci);
            tekuci=tekuci->sljedeci;
            dodajIspred(el);
            tekuci=temp; 
        }
        
    }
    
    tip operator [] (int i) const override {
        if (i<0 || i>=duzina) throw std::domain_error("Neispravan argument");;
        Cvor *p(prvi);
        for (int j=0; j<i; j++) p=p->sljedeci;
        return p->element;
    }
    
    tip &operator [] (int i) override {
        if (i<0 || i>=duzina) throw std::domain_error("Neispravan argument");;
        Cvor *p(prvi);
        for (int j=0; j<i; j++) p=p->sljedeci;
        return p->element;
    }

};

void TestKonstruktoraZaNizListu() {
    NizLista<int> l;
    if (l.brojElemenata()!=0) std::cout << "Test konstruktora: Greška" << std::endl;
    else std::cout << "Test konstruktora: OK" << std::endl;
}
void TestKonstruktoraZaJednostrukuListu() {
    JednostrukaLista<int> l;
    if (l.brojElemenata()!=0) std::cout << "Test konstruktora: Greška" << std::endl;
    else std::cout << "Test konstruktora: OK" << std::endl;
}
void TestKopirajucegKonstruktoraNizLista() {
    NizLista<int> l1;
    for (int i=0; i<5; i++) l1.dodajIspred(i+1);
    NizLista<int> l2(l1);
    l1.obrisi();
    if (l1.brojElemenata()==l2.brojElemenata()) std::cout << "Test kopirajućeg konstruktora: Greska" << std::endl;
    else std::cout << "Test kopirajućeg konstruktora: OK" << std::endl;
}
void TestOperatoraDodjeleNizLista() {
    NizLista<int> l1;
    for (int i=0; i<5; i++) l1.dodajIspred(i+1);
    NizLista<int> l2;
    for (int i=0; i<8; i++) l2.dodajIspred(i*2);
    l2=l1;
    if (l1.brojElemenata()!=l2.brojElemenata()) std::cout << "Test operatora dodjele: Greska" << std::endl;
    else std::cout << "Test operatora dodjele: OK" << std::endl;
}
void TestKopirajucegKonstruktoraJednostrukaLista() {
    JednostrukaLista<int> l1;
    for (int i=0; i<5; i++) l1.dodajIspred(i+1);
    JednostrukaLista<int> l2(l1);
    l1.obrisi();
    if (l1.brojElemenata()==l2.brojElemenata()) std::cout << "Test kopirajućeg konstruktora: Greska" << std::endl;
    else std::cout << "Test kopirajućeg konstruktora: OK" << std::endl;
}
void TestOperatoraDodjeleJednostrukaLista() {
    JednostrukaLista<int> l1;
    for (int i=0; i<5; i++) l1.dodajIspred(i+1);
    JednostrukaLista<int> l2;
    for (int i=0; i<8; i++) l2.dodajIspred(i*2);
    l2=l1;
    if (l1.brojElemenata()!=l2.brojElemenata()) std::cout << "Test operatora dodjele: Greska" << std::endl;
    else std::cout << "Test operatora dodjele: OK" << std::endl;
}
void TestBrojaElemenata(const Lista<int> &l, int n) {
    if (l.brojElemenata()!=n) std::cout << "Test broja elemenata: Greska" << std::endl;
    else std::cout << "Test broja elemenata: OK" << std::endl;
}
void TestTrenutnog(Lista<int> &l) {
    l.dodajIspred(5);
    l.prethodni();
    if (l.trenutni()!=5) std::cout << "Test trenutnog elementa: Greska" << std::endl;
    else std::cout << "Test trenutnog elemenata: OK" << std::endl;
}
void TestTrenutnogIzuzetak(Lista<int> &l) {
    while (l.brojElemenata()) l.obrisi();
    try {
        l.trenutni();
    }
    catch(std::logic_error &c) {
        std::cout << "Test trenutnog elemenata sa izuzetkom: OK" << std::endl;
        return;
    }
    catch(...) {}
    std::cout << "Test trenutnog elemenata sa izuzetkom: Greska" << std::endl;
}
void TestPrethodnog(Lista<int> &l) {
    l.dodajIza(3); l.dodajIspred(4);
    l.prethodni();
    if (l.trenutni()!=4) std::cout << "Test prethodnog elementa: Greska" << std::endl;
    else std::cout << "Test prethodnog elemenata: OK" << std::endl;
}
void TestPrethodnogSaIzuzetkom(Lista<int> &l) {
    l.pocetak();
    if (l.prethodni()) std::cout << "Test prethodnog elementa sa izuzetkom: Greska" << std::endl;
    else std::cout << "Test prethodnog elemenata sa izuzetkom: OK" << std::endl;
}
void TestSljedeceg(Lista<int> &l) {
    l.dodajIspred(3); l.dodajIza(4);
    l.sljedeci();
    if (l.trenutni()!=4) std::cout << "Test sljedeceg elementa: Greska" << std::endl;
    else std::cout << "Test sljedeceg elemenata: OK" << std::endl;
}
void TestSljedecegSaIzuzetkom(Lista<int> &l) {
    l.dodajIspred(7);
    l.kraj();
    if (l.sljedeci()) std::cout << "Test sljedeceg elementa sa izuzetkom: Greska" << std::endl;
    else std::cout << "Test sljedeceg elemenata sa izuzetkom: OK" << std::endl;
}
void TestPocetka(Lista<int> &l) {
    while (l.prethodni());
    l.dodajIspred(16);
    l.pocetak();
    if (l.trenutni()!=16) std::cout << "Test pocetnog elementa: Greska" << std::endl;
    else std::cout << "Test pocetnog elementa: OK" << std::endl;
}
void TestKraja(Lista<int> &l) {
    l.dodajIspred(7);
    while (l.sljedeci());
    l.dodajIza(16);
    l.kraj();
    if (l.trenutni()!=16) std::cout << "Test krajnjeg elementa: Greska" << std::endl;
    else std::cout << "Test krajnjeg elementa: OK" << std::endl;
}
void TestBrisanja(Lista<int> &l) {
    l.dodajIspred(7);
    l.dodajIza(8);
    l.obrisi();
    if (l.trenutni()!=8) std::cout << "Test brisanja elementa: Greska" << std::endl;
    else std::cout << "Test brisanja elementa: OK" << std::endl;
}
void TestBrisanjaZadnjeg(Lista<int> &l) {
    l.dodajIspred(7);
    while (l.sljedeci());
    l.dodajIspred(8);
    l.obrisi();
    if (l.trenutni()!=8) std::cout << "Test brisanja zadnjeg elementa: Greska" << std::endl;
    else std::cout << "Test brisanja zadnjeg elementa: OK" << std::endl;
}
void TestDodajIspred(Lista<int> &l) {
    l.dodajIspred(7); l.dodajIspred(8);
    l.prethodni();
    if (l.trenutni()!=8) std::cout << "Test dodavanja elementa ispred: Greska" << std::endl;
    else std::cout << "Test dodavanja elementa ispred: OK" << std::endl;
}
void TestDodajIspredPrazna(Lista<int> &l) {
    while (l.brojElemenata()) l.obrisi();
    l.dodajIspred(8);
    if (l.trenutni()!=8) std::cout << "Test dodavanja elementa ispred na praznu listu: Greska" << std::endl;
    else std::cout << "Test dodavanja elementa ispred na praznu listu: OK" << std::endl;
}
void TestDodajIza(Lista<int> &l) {
    l.dodajIza(7); l.dodajIza(8);
    l.sljedeci();
    if (l.trenutni()!=8) std::cout << "Test dodavanja elementa iza: Greska" << std::endl;
    else std::cout << "Test dodavanja elementa iza: OK" << std::endl;
}
void TestDodajIzaPrazna(Lista<int> &l) {
    while (l.brojElemenata()) l.obrisi();
    l.dodajIza(8);
    if (l.trenutni()!=8) std::cout << "Test dodavanja elementa iza na praznu listu: Greska" << std::endl;
    else std::cout << "Test dodavanja elementa iza na praznu listu: OK" << std::endl;
}
void TestOperatora(Lista<int> &l) {
    l.dodajIza(8);
    l.pocetak();
    l.dodajIza(1);
    l.dodajIza(7);
    if (l[1]!=7) std::cout << "Test operatora []: Greska" << std::endl;
    else std::cout << "Test operatora []: OK" << std::endl;
}
void TestOperatoraVanOpsega(Lista<int> &l) {
    while (l.brojElemenata()!=0) l.obrisi();
    l.dodajIza(7); l.dodajIza(3);
    try {
        l[3];
    }
    catch(std::domain_error) {
        std::cout << "Test operatora [] van opsega: OK" << std::endl;
        return;
    }
    catch(...) {}
    std::cout << "Test operatora [] van opsega: Greska" << std::endl;
}


int main() {
    
    // TESTIRANJE NIZ LISTE
    
    std::cout << "Testovi za niz listu: " << std::endl << std::endl;
    
    TestKonstruktoraZaNizListu();
    TestKopirajucegKonstruktoraNizLista();
    TestOperatoraDodjeleNizLista();
    
    NizLista<int> niz; for (int i=0; i<7; i++) niz.dodajIza(5);
    
    TestBrojaElemenata(niz, 7);
    TestTrenutnog(niz);
    TestTrenutnogIzuzetak(niz);
    TestPrethodnog(niz);
    TestPrethodnogSaIzuzetkom(niz);
    TestSljedeceg(niz);
    TestSljedecegSaIzuzetkom(niz);
    TestPocetka(niz);
    TestKraja(niz);
    TestBrisanja(niz);
    TestBrisanjaZadnjeg(niz);
    TestDodajIspred(niz);
    TestDodajIspredPrazna(niz);
    TestDodajIza(niz);
    TestDodajIzaPrazna(niz);
    TestOperatora(niz);
    TestOperatoraVanOpsega(niz);
    
    
    // TESTIRANJE JEDNOSTRUKE LISTE
    
    std::cout << std::endl << "Testovi za jednostruku listu: " << std::endl << std::endl;
    
    TestKonstruktoraZaJednostrukuListu();
    TestKopirajucegKonstruktoraJednostrukaLista();
    TestOperatoraDodjeleJednostrukaLista();
    
    JednostrukaLista<int> lista; for(int i=0; i<7; i++) lista.dodajIspred(8);
    
    TestBrojaElemenata(lista, 7);
    TestTrenutnog(lista);
    TestTrenutnogIzuzetak(lista);
    TestPrethodnog(lista);
    TestPrethodnogSaIzuzetkom(lista);
    TestSljedeceg(lista);
    TestSljedecegSaIzuzetkom(lista);
    TestPocetka(lista);
    TestKraja(lista);
    TestBrisanja(lista);
    TestBrisanjaZadnjeg(lista);
    TestDodajIspred(lista);
    TestDodajIspredPrazna(lista);
    TestDodajIza(lista);
    TestDodajIzaPrazna(lista);
    TestOperatora(lista);
    TestOperatoraVanOpsega(lista);
    
    return 0;
}
