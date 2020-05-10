#include <iostream>
#include <stdexcept>
#include <typeinfo>

template <typename tip>  
class Iterator;

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
class DvostrukaLista : public Lista<tip> {
    
    struct Cvor { 
        tip element; 
        Cvor *sljedeci=nullptr; 
        Cvor *prethodni=nullptr;
        Cvor (const tip &el, Cvor* pret, Cvor *sljed) {
            element=el;
            sljedeci=sljed;
            prethodni=pret;
            if (prethodni) prethodni->sljedeci=this;
            if (sljedeci) sljedeci->prethodni=this;
        }
        Cvor(Cvor* pret=nullptr, Cvor* sljed=nullptr) {
            sljedeci=sljed;
            prethodni=pret;
            if (prethodni) prethodni->sljedeci = this;
            if (sljedeci) sljedeci->prethodni = this;
        }
    };
    Cvor* prvi=nullptr;
    Cvor* zadnji=nullptr;
    Cvor* tekuci=nullptr;
    int lduzina;
    int dduzina;
    
    friend class Iterator<tip>;
    
    void ObrisiListu() {
        while(prvi) {
            tekuci = prvi;
            prvi = prvi->sljedeci;
            delete tekuci;
        }
        zadnji=nullptr;
        tekuci=nullptr;
        prvi=nullptr;
        lduzina=0;
        dduzina=0;
    }
    
    void Kreiraj() {
        prvi=new Cvor(nullptr, nullptr);
        tekuci=prvi;
        zadnji=new Cvor (prvi, nullptr);
        prvi->sljedeci = zadnji;
        tekuci->sljedeci=zadnji;
        lduzina=0; 
        dduzina=0; 
    }
    
    public:
    
    DvostrukaLista() {
        Kreiraj();
    }
    
    ~DvostrukaLista() {
        ObrisiListu();
    }
    
    DvostrukaLista(const DvostrukaLista<tip> &l) {
        if (l.dduzina) {
            
            Kreiraj();
            
            dduzina=l.dduzina;
            lduzina=l.lduzina;
            
            Cvor* p1(l.prvi), *p2(prvi);
            
            while (p1->sljedeci!=l.zadnji) {
                
                new Cvor(p1->sljedeci->element, tekuci, tekuci->sljedeci);
                
                if (p1==l.tekuci) p2=tekuci;
                
                tekuci=tekuci->sljedeci;
                p1=p1->sljedeci;
            }
            //zadnji->prethodni=tekuci;
            tekuci=p2;
        }
        else Kreiraj();
    }
    
    DvostrukaLista(DvostrukaLista<tip> &&l) {
        dduzina=l.dduzina;
        lduzina=l.lduzina;
        prvi=l.prvi;
        tekuci=l.tekuci;
        zadnji=l.zadnji;
        l.prvi=nullptr; l.tekuci=nullptr; l.zadnji=nullptr; l.dduzina=0; l.lduzina=0;
    }
    
    DvostrukaLista &operator =(const DvostrukaLista<tip> &l) {
        
        if (l.prvi!=prvi) {
            ObrisiListu();
            
            DvostrukaLista temp(l);
            *this=std::move(temp);
        }
        return *this;
    }
    
    DvostrukaLista &operator =(DvostrukaLista<tip> &&l) {
        if (l.prvi!=prvi) {
            dduzina=l.dduzina;
            lduzina=l.lduzina;
            prvi=l.prvi;
            tekuci=l.tekuci;
            zadnji=l.zadnji;
            l.prvi=nullptr; l.tekuci=nullptr; l.zadnji=nullptr; l.dduzina=0; l.lduzina=0;                                               
        }
        return *this;
    }
    
    int brojElemenata() const override {
        return dduzina+lduzina;
    }
    
    tip trenutni() const override {
        if (dduzina==0) throw std::logic_error("Prazna lista");
        return tekuci->sljedeci->element;
    }
    
    tip &trenutni() override {
        if (dduzina==0) throw std::logic_error("Prazna lista");
        return tekuci->sljedeci->element;
    }
    
    bool prethodni() override {
        
        if (tekuci != prvi) {
            tekuci = tekuci->prethodni;
            lduzina--;
            dduzina++;
            return true;
        }
        return false;
    }
    
    bool sljedeci() override {
        
        if (dduzina!=0 && dduzina!=1) {
            tekuci = tekuci->sljedeci;
            dduzina--;
            lduzina++;
            return true;
        }
        return false;
    }
    
    void pocetak() override {
        if (!dduzina) throw std::logic_error("Prazna lista");
        tekuci = prvi;
        dduzina+=lduzina;
        lduzina = 0;
    }
    
    void kraj() override {
        if (!dduzina) throw std::logic_error("Prazna lista");
        tekuci = zadnji->prethodni->prethodni;
        lduzina+=dduzina-1;
        dduzina=1;
    }
    
    void obrisi() override {
        if (!dduzina) throw std::logic_error("Prazna lista");
        
        Cvor* p=tekuci->sljedeci;
        p->sljedeci->prethodni = tekuci;
        tekuci->sljedeci=p->sljedeci;
        delete p;
        dduzina--;
        if (tekuci->sljedeci==zadnji && tekuci!=prvi) {
            tekuci=tekuci->prethodni;
            dduzina++; lduzina--;
        }
    }
    
    void dodajIspred(const tip &el) override {
        new Cvor(el, tekuci, tekuci->sljedeci);
        if (dduzina) tekuci=tekuci->sljedeci;
        else { dduzina++; return; }
        lduzina++;
    }
    
    void dodajIza(const tip &el) override {
        if (!dduzina) new Cvor(el, tekuci, tekuci->sljedeci);
        else new Cvor(el, tekuci->sljedeci, tekuci->sljedeci->sljedeci);
        dduzina++;
    }
    
    tip operator [] (int i) const override {
        Cvor* p=tekuci;
        if (i<0 || i>=lduzina+dduzina) throw std::domain_error("Neispravan indeks");
        
        int k(0);
        
        if (i<=lduzina/2) {
            k=i;
            p=prvi;
        }
        else if (i>lduzina/2 && i<lduzina+dduzina-dduzina/2) {
            k=i-lduzina;
        }
        else {
            k=i-lduzina-dduzina;
            p=zadnji->prethodni;
        }
        
        if (k<0) {
            for (int i=0; i<abs(k); i++) p = p->prethodni;
        }
        else {
            for (int i=0; i<k; i++) {
                p = p->sljedeci;
            }
        }
        
        return p->sljedeci->element;
    }
    
    tip &operator [] (int i) override {
        Cvor* p=tekuci;
        if (i<0 || i>=lduzina+dduzina) throw std::domain_error("Neispravan indeks");
        
        int k(0);
        
        if (i<=lduzina/2) {
            k=i;
            p=prvi;
        }
        else if (i>lduzina/2 && i<lduzina+dduzina-dduzina/2) {
            k=i-lduzina;
        }
        else {
            k=i-lduzina-dduzina;
            p=zadnji->prethodni;
        }
        
        if (k<0) {
            for (int i=0; i<abs(k); i++) p = p->prethodni;
        }
        else {
            for (int i=0; i<k; i++) {
                p = p->sljedeci;
            }
        }
        
        return p->sljedeci->element;
    }

};

template <typename tip>  
class Iterator {
    const DvostrukaLista<tip> *lista;
    typename DvostrukaLista<tip>::Cvor *trenutniDL;
    
    public:
    
    Iterator(const DvostrukaLista<tip> &l) {
        lista=&l;
        trenutniDL=l.tekuci;
    }
    
    tip trenutni() const {
        if (lista->brojElemenata()==0) throw "Prazna lista";
        return trenutniDL->sljedeci->element;
    }
    
    bool prethodni() {
        
        if (lista->brojElemenata()==0) throw "Prazna lista";
        if (trenutniDL==lista->prvi) return false;
        trenutniDL=trenutniDL->prethodni;            
        return true;
    }
    
    bool sljedeci() {
        if (lista->brojElemenata()==0) throw "Prazna lista";
        if (trenutniDL->sljedeci->sljedeci==lista->zadnji) return false;
        trenutniDL=trenutniDL->sljedeci;
        return true;
    }
    
    void pocetak() {
        if (lista->brojElemenata()==0) throw "Prazna lista";
        trenutniDL=lista->prvi;
    }
    
    void kraj() {
        if (lista->brojElemenata()==0) throw "Prazna lista";
        trenutniDL=lista->zadnji->prethodni->prethodni;
    }
};


template <typename tip>
tip dajMaksimum(const Lista<tip>& n) {
    try {
        if(typeid(n) == typeid(DvostrukaLista<tip>)) {
            Iterator<tip> it(static_cast<DvostrukaLista<tip>&>(const_cast<Lista<tip>&>(n)));
            it.pocetak();
            tip max(it.trenutni());
            while (it.sljedeci()) if (it.trenutni()>max) max=it.trenutni();
            return max;
        } 
    }
    catch(...) {
        throw;
    }
}

void TestKonstruktora() {
    DvostrukaLista<int> l;
    if (l.brojElemenata()!=0) std::cout << "Test konstruktora: Greška" << std::endl;
    else std::cout << "Test konstruktora: OK" << std::endl;
}


void TestKopirajucegKonstruktora() {
    DvostrukaLista<int> l1;
    for (int i=0; i<5; i++) l1.dodajIspred(i+1);
    DvostrukaLista<int> l2(l1);
    l1.obrisi();
    if (l1.brojElemenata()==l2.brojElemenata()) std::cout << "Test kopirajućeg konstruktora: Greska" << std::endl;
    else std::cout << "Test kopirajućeg konstruktora: OK" << std::endl;
}

void TestOperatoraDodjele() {
    DvostrukaLista<int> l1;
    for (int i=0; i<5; i++) l1.dodajIspred(i+1);
    DvostrukaLista<int> l2;
    for (int i=0; i<8; i++) l2.dodajIspred(i*2);
    l2=l1;
    if (l2.brojElemenata()!=5 ) std::cout << "Test operatora dodjele: Greska" << std::endl;
    else std::cout << "Test operatora dodjele: OK" << std::endl;
}

void TestSamododjele() {
    DvostrukaLista<int> l1;
    for (int i=0; i<5; i++) l1.dodajIspred(i+1);
    try {
        l1=l1;
        l1.trenutni();
        if (l1.brojElemenata()==5) { std::cout << "Test samododjele: OK" << std::endl; return; }
    }
    catch(...) {}
    std::cout << "Test samododjele: Greska" << std::endl;
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

void TestDajMaksimum1(Lista<int> &l) {
    while (l.brojElemenata()!=0) l.obrisi();
    l.dodajIza(7); l.dodajIza(3); l.dodajIspred(12); l.dodajIza(2);
    l.kraj();
    if (dajMaksimum(l)!=12) std::cout << "Test funkcije dajMaksimum 1: Greska" << std::endl;
    else std::cout << "Test funkcije dajMaksimum 1: OK" << std::endl;
}

void TestDajMaksimum2(Lista<int> &l) {
    while (l.brojElemenata()!=0) l.obrisi();
    try {
        dajMaksimum(l);
    }
    catch(const char *c) {
        std::cout << "Test funkcije dajMaksimum 2: OK" << std::endl;
        return;
    }
    catch(...) {}
    std::cout << "Test funkcije dajMaksimum 2: Greska" << std::endl;
} 


int main() {
    
    TestKonstruktora();
    TestKopirajucegKonstruktora();
    TestOperatoraDodjele();
    TestSamododjele();
    
    DvostrukaLista<int> lista; for(int i=0; i<7; i++) lista.dodajIspred(8);
    
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
    TestDajMaksimum1(lista);
    TestDajMaksimum2(lista);
    
    return 0;
}
