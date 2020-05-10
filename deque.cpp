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
        tekuci = prvi;
        dduzina+=lduzina;
        lduzina = 0;
    }
    
    void kraj() override {
        if (!dduzina) { tekuci=prvi; lduzina=dduzina=0; }
        else { tekuci = zadnji->prethodni->prethodni; lduzina+=dduzina-1; dduzina=1; }
        
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
class DvostraniRed {
    DvostrukaLista<tip> l;
    
    public:
    DvostraniRed() {}
    ~DvostraniRed() {}
    
    DvostraniRed (const DvostraniRed &r) { 
        l=r.l;
    }
    
    DvostraniRed (DvostraniRed &&r) {
        l=std::move(r.l);
    }
    
    DvostraniRed &operator =(const DvostraniRed &r) {
        if (&r == this) return *this;
        l=r.l;
        return *this;
    }
    DvostraniRed &operator =(DvostraniRed &&r) {
        if (&r == this) return *this;
        l=std::move(r.l);
        return *this;
    }
    
    void brisi() { l.ObrisiListu(); l.Kreiraj(); }
    
    int brojElemenata() const { return l.brojElemenata(); }
    
    void staviNaVrh (const tip &element) {
        l.kraj();
        l.dodajIza(element);
    }
    
    tip skiniSaVrha() {
        if (l.brojElemenata()==0) throw std::logic_error("Red je prazan!");
        l.kraj();
        tip el(l.trenutni());
        l.obrisi();
        return el;
    }
    
    void staviNaCelo(const tip &element) {
        l.pocetak();
        l.dodajIspred(element);
    }
    
    tip skiniSaCela() {
        if (l.brojElemenata()==0) throw std::logic_error("Red je prazan!");
        l.pocetak();
        tip el(l.trenutni());
        l.obrisi();
        return el;
    }
    
    tip& celo() {
        if (l.brojElemenata()==0) throw std::logic_error("Red je prazan!");
        l.pocetak();
        return l.trenutni();
    }
    
    tip& vrh() {
        if (l.brojElemenata()==0) throw std::logic_error("Red je prazan!");
        l.kraj();
        return l.trenutni();
    }
};

void TestKonstruktora() {
    DvostraniRed<int> r;
    if (r.brojElemenata()==0) std::cout << "Test konstruktora: OK" << std::endl;
    else std::cout << "Test konstruktora: Greska" << std::endl;
}

void TestKopirajucegKonstruktora() {
    DvostraniRed<int> r1;
    for (int i=0; i<5; i++) r1.staviNaCelo(i+2);
    DvostraniRed<int> r2(r1);
    r1.skiniSaCela();
    if (r2.brojElemenata()==5) std::cout << "Test kopirajuceg konstruktora: OK" << std::endl;
    else std::cout << "Test kopirajuceg konstruktora: Greska" << std::endl;
}

void TestOperatoraDodjele() {
    DvostraniRed<int> r1;
    for (int i=0; i<5; i++) r1.staviNaCelo(i+2);
    DvostraniRed<int> r2;
    for (int i=0; i<10; i++) r2.staviNaCelo(i+2);
    r1=r2;
    r2.skiniSaCela();
    if (r1.brojElemenata()==10 && r2.brojElemenata()==9) std::cout << "Test operatora dodjele: OK" << std::endl;
    else std::cout << "Test operatora dodjele: Greska" << std::endl;
}

void TestBrojaElemenata(DvostraniRed<int> &r, int n) {
    if (r.brojElemenata()==n) std::cout << "Test broja elementa: OK" << std::endl;
    else std::cout << "Test broja elementa: Greska" << std::endl;
}

void TestStavljanjaNaCelo(DvostraniRed<int> &r) {
    int n(r.brojElemenata());
    r.staviNaCelo(8);
    if (r.brojElemenata()==n+1) std::cout << "Test stavljanja elementa na celo: OK" << std::endl;
    else std::cout << "Test stavljanja elementa na celo: Greska" << std::endl;
} 

void TestStavljanjaNaVrh(DvostraniRed<int> &r) {
    int n(r.brojElemenata());
    r.staviNaVrh(8);
    if (r.brojElemenata()==n+1) std::cout << "Test stavljanja elementa na vrh: OK" << std::endl;
    else std::cout << "Test stavljanja elementa na vrh: Greska" << std::endl;
} 

void TestSkidanjaSaCela(DvostraniRed<int> &r) {
    int y(r.celo());
    int x(r.skiniSaCela());
    if (x==y) std::cout << "Test skidanja elementa sa cela: OK" << std::endl;
    else std::cout << "Test skidanja elementa sa cela: Greska" << std::endl;
}

void TestSkidanjaSaVrha(DvostraniRed<int> &r) {
    int y(r.vrh());
    int x(r.skiniSaVrha());
    if (x==y) std::cout << "Test skidanja elementa sa vrha: OK" << std::endl;
    else std::cout << "Test skidanja elementa sa vrha: Greska" << std::endl;
}

void TestCela(DvostraniRed<int> &r) {
    r.staviNaCelo(8);
    if (r.celo()==8) std::cout << "Test cela: OK" << std::endl;
    else std::cout << "Test cela: Greska" << std::endl;
}

void TestVrha(DvostraniRed<int> &r) {
    r.staviNaVrh(8);
    if (r.vrh()==8) std::cout << "Test vrha: OK" << std::endl;
    else std::cout << "Test vrha: Greska" << std::endl;
}


void TestBrisanja(DvostraniRed<int> &r) {
    r.brisi();
    if (r.brojElemenata()==0) std::cout << "Test brisanja reda: OK" << std::endl;
    else std::cout << "Test brisanja reda: Greska" << std::endl;
}


int main() {
    
    TestKonstruktora();
    TestKopirajucegKonstruktora();
    TestOperatoraDodjele();
    
    DvostraniRed<int> r;
    for (int i=0; i<1; i++) r.staviNaVrh(i+9);
    
    TestBrojaElemenata(r, 1);
    TestStavljanjaNaCelo(r);
    TestStavljanjaNaVrh(r);
    TestSkidanjaSaCela(r);
    TestSkidanjaSaVrha(r);
    TestCela(r);
    TestVrha(r);
    TestBrisanja(r);
    
    return 0;
}



