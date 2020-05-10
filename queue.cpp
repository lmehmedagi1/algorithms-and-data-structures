#include <iostream>
#include <stdexcept>


template <typename tip>
class Red {
    struct Cvor {
        tip element;
        Cvor *sljedeci;
        Cvor (const tip &element, Cvor *sljedeci) {
            this->element=element;
            this->sljedeci=sljedeci;
        }
    };
    
    Cvor *pocetak; 
    Cvor *kraj;
    int velicina;
    
    void Unisti() { 
        while(!JeLiPrazan()) skini();
        pocetak=nullptr; kraj=nullptr; velicina=0;
    }
    
    void Kopiraj (const Red &r) {
        Cvor *p(r.pocetak);
        while(p!=nullptr) {
            stavi(p->element);
            p=p->sljedeci;
        }
    }
    
    bool JeLiPrazan() const { return pocetak==nullptr; }
    
    public:
    Red() : pocetak(nullptr), kraj(nullptr), velicina(0) {}
    ~Red() { Unisti(); }
    
    Red (const Red &r) { 
        pocetak=nullptr; kraj=nullptr; velicina=0;
        Kopiraj(r);
    }
    
    Red (Red &&r) {
        pocetak=r.pocetak; r.pocetak=nullptr;
        kraj=r.kraj; r.kraj=nullptr;
        velicina=r.velicina; r.velicina=0;
    }
    
    Red &operator =(const Red &r) {
        if (&r == this) return *this;
        Unisti();
        Kopiraj(r);
        return *this;
    }
    Red &operator =(Red &&r) {
        if (&r == this) return *this;
        Unisti();
        pocetak=r.pocetak; r.pocetak=nullptr;
        kraj=r.kraj; r.kraj=nullptr;
        velicina=r.velicina; r.velicina=0;
        return *this;
    }
    
    void brisi() { Unisti(); }
    
    void stavi (const tip &element) {
        Cvor *n(new Cvor(element, nullptr));
        if (pocetak==nullptr) {
            kraj=n;
            pocetak=n;
        }
        else {
            kraj->sljedeci=n;
            kraj = kraj->sljedeci;
        }
        velicina++;
    }
    
    tip skini() {
        if (pocetak==nullptr) throw std::logic_error("Red je prazan!");
        tip element(pocetak->element);
        Cvor *p=pocetak;
        if (pocetak == kraj) {
            kraj=nullptr;
            pocetak=kraj;
        }
        else pocetak=pocetak->sljedeci;
        delete p;
        velicina--;
        return element;
    }
    
    tip& celo() const {
        if (pocetak==nullptr) throw std::logic_error("Red je prazan!");
        return pocetak->element;
    }
    int brojElemenata() const { return velicina; }
};

void TestKonstruktora() {
    Red<int> r;
    if (r.brojElemenata()==0) std::cout << "Test konstruktora: OK" << std::endl;
    else std::cout << "Test konstruktora: Greska" << std::endl;
}
void TestKopirajucegKonstruktora() {
    Red<int> r1;
    for (int i=0; i<5; i++) r1.stavi(i+2);
    Red<int> r2(r1);
    r1.skini();
    if (r2.brojElemenata()==5) std::cout << "Test kopirajuceg konstruktora: OK" << std::endl;
    else std::cout << "Test kopirajuceg konstruktora: Greska" << std::endl;
}
void TestOperatoraDodjele() {
    Red<int> r1;
    for (int i=0; i<5; i++) r1.stavi(i+2);
    Red<int> r2;
    for (int i=0; i<10; i++) r2.stavi(i+2);
    r1=r2;
    r2.skini();
    if (r1.brojElemenata()==10 && r2.brojElemenata()==9) std::cout << "Test operatora dodjele: OK" << std::endl;
    else std::cout << "Test operatora dodjele: Greska" << std::endl;
}
void TestBrojaElemenata(Red<int> &r, int n) {
    if (r.brojElemenata()==n) std::cout << "Test broja elementa: OK" << std::endl;
    else std::cout << "Test broja elementa: Greska" << std::endl;
}
void TestStavljanja(Red<int> &r) {
    int n(r.brojElemenata());
    r.stavi(8);
    if (r.brojElemenata()==n+1) std::cout << "Test stavljanja elementa: OK" << std::endl;
    else std::cout << "Test stavljanja elementa: Greska" << std::endl;
} 
void TestSkidanja(Red<int> &r) {
    int y(r.celo());
    int x(r.skini());
    if (x==y) std::cout << "Test skidanja elementa: OK" << std::endl;
    else std::cout << "Test skidanja elementa: Greska" << std::endl;
}
void TestCela(Red<int> &r) {
    r.stavi(8);
    if (r.celo()==8) std::cout << "Test cela: OK" << std::endl;
    else std::cout << "Test cela: Greska" << std::endl;
}
void TestBrisanja(Red<int> &r) {
    r.brisi();
    if (r.brojElemenata()==0) std::cout << "Test brisanja reda: OK" << std::endl;
    else std::cout << "Test brisanja reda: Greska" << std::endl;
}

int main() {
    TestKonstruktora();
    TestKopirajucegKonstruktora();
    TestOperatoraDodjele();
    
    Red<int> r;
    for (int i=0; i<1; i++) r.stavi(i+9);
    
    TestBrojaElemenata(r, 1);
    TestStavljanja(r);
    TestSkidanja(r);
    TestCela(r);
    TestBrisanja(r);
    
    return 0;
}
