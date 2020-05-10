#include <iostream>
#include <stdexcept>

template <typename tip>
class Stek {
    struct Cvor { 
        tip element; 
        Cvor *sljedeci;
        Cvor (const tip &element, Cvor *sljedeci) : element(element), sljedeci(sljedeci) {}
    };
    Cvor * zadnji;
    int velicina;
    void Kopiraj(const Stek &s) {
        Cvor *q, *p(s.zadnji);
        zadnji = nullptr;
        velicina = s.velicina;
        while(p != nullptr) {
            Cvor *n(new Cvor(p->element, nullptr));
            if (zadnji==nullptr) zadnji = n;
            else q->sljedeci = n;
            p = p->sljedeci;
            q = n;
        }
    }
    
    public:
    Stek() : zadnji(nullptr), velicina(0) {} 
    ~Stek() { brisi(); }
    
    
    Stek(const Stek &s) { Kopiraj(s); }
    Stek(Stek &&s) {
        zadnji=s.zadnji;
        velicina=s.velicina;
        s.zadnji=nullptr; s.velicina=0;
    }
    
    Stek &operator =(const Stek &s) {
        if (&s == this) return *this;
        brisi();
        Kopiraj(s);
        return *this;
    }
    
    Stek &operator =(Stek &&s) {
        zadnji=s.zadnji;
        velicina=s.velicina;
        s.zadnji=nullptr; s.velicina=0;
        return *this;
    }
    
    void stavi(const tip &element) {
        zadnji = new Cvor(element, zadnji); velicina++;
    }
    
    tip skini() {
        if (zadnji == nullptr) throw std::logic_error("Stek je prazan!");
        tip element(zadnji->element);
        Cvor *p = zadnji->sljedeci;
        delete zadnji;
        zadnji = p;
        velicina--;
        return element;
    }

    tip& vrh() const  {
        if (zadnji==nullptr) throw std::logic_error("Stek je prazan!");
        return zadnji->element;
    }

    void brisi()  { while(!JeLiPrazan()) skini(); }
    bool JeLiPrazan() const { return zadnji==nullptr; }
    int brojElemenata() const { return velicina; } 
};

void TestKonstruktora() {
    Stek<int> s;
    if (s.JeLiPrazan()) std::cout << "Test konstruktora: OK" << std::endl;
    else std::cout << "Test konstruktora: Greska" << std::endl;
}

void TestKopirajucegKonstruktora() {
    Stek<int> s1;
    for (int i=0; i<5; i++) s1.stavi(i+2);
    Stek<int> s2(s1);
    s1.skini();
    if (s2.brojElemenata()==5) std::cout << "Test kopirajuceg konstruktora: OK" << std::endl;
    else std::cout << "Test kopirajuceg konstruktora: Greska" << std::endl;
}

void TestOperatoraDodjele() {
    Stek<int> s1;
    for (int i=0; i<5; i++) s1.stavi(i+2);
    Stek<int> s2;
    for (int i=0; i<10; i++) s2.stavi(i+2);
    s1=s2;
    s2.skini();
    if (s1.brojElemenata()==10 && s2.brojElemenata()==9) std::cout << "Test operatora dodjele: OK" << std::endl;
    else std::cout << "Test operatora dodjele: Greska" << std::endl;
}

void TestBrojaElemenata(Stek<int> &s, int n) {
    if (s.brojElemenata()==n) std::cout << "Test broja elementa: OK" << std::endl;
    else std::cout << "Test broja elementa: Greska" << std::endl;
}

void TestStavljanja(Stek<int> &s) {
    int n(s.brojElemenata());
    s.stavi(8);
    int x(s.vrh());
    if (x==8 && s.brojElemenata()==n+1) std::cout << "Test stavljanja elementa: OK" << std::endl;
    else std::cout << "Test stavljanja elementa: Greska" << std::endl;
} 

void TestSkidanja(Stek<int> &s) {
    s.stavi(8);
    int x(s.skini());
    if (x==8) std::cout << "Test skidanja elementa: OK" << std::endl;
    else std::cout << "Test skidanja elementa: Greska" << std::endl;
}

void TestSkidanjaSaIzuzetkom(Stek<int> &s) {
    s.brisi();
    try {
        s.skini();
    }
    catch(std::logic_error &x) {
        std::cout << "Test skidanja elementa sa izuzetkom: OK" << std::endl;
        return;
    }
    catch(...) {}
    std::cout << "Test skidanja elementa sa izuzetkom: Greska" << std::endl;
}

void TestVrha(Stek<int> &s) {
    s.stavi(8);
    if (s.vrh()==8) std::cout << "Test vrha: OK" << std::endl;
    else std::cout << "Test vrha: Greska" << std::endl;
}

void TestVrhaSaIzuzetkom(Stek<int> &s) {
    s.brisi();
    try {
        s.vrh();
    }
    catch(std::logic_error &x) {
        std::cout << "Test vrha sa izuzetkom: OK" << std::endl;
        return;
    }
    catch(...) {}
    std::cout << "Test vrha sa izuzetkom: Greska" << std::endl;
}

void TestBrisanja(Stek<int> &s) {
    s.brisi();
    if (s.brojElemenata()==0) std::cout << "Test brisanja steka: OK" << std::endl;
    else std::cout << "Test brisanja steka: Greska" << std::endl;
}

void TestPraznog1(Stek<int> &s) {
    s.brisi();
    if (s.JeLiPrazan()) std::cout << "Test provjere praznog steka 1: OK" << std::endl;
    else std::cout << "Test provjere praznog steka 1: Greska" << std::endl;
}

void TestPraznog2(Stek<int> &s) {
    s.stavi(8);
    if (!s.JeLiPrazan()) std::cout << "Test provjere praznog steka 2: OK" << std::endl;
    else std::cout << "Test provjere praznog steka 2: Greska" << std::endl;
}


int main() {
    TestKonstruktora();
    TestKopirajucegKonstruktora();
    TestOperatoraDodjele();
    
    Stek<int> s;
    for (int i=0; i<1; i++) s.stavi(i+9);
    
    TestBrojaElemenata(s, 1);
    TestStavljanja(s);
    TestSkidanja(s);
    TestSkidanjaSaIzuzetkom(s);
    TestVrha(s);
    TestVrhaSaIzuzetkom(s);
    TestBrisanja(s);
    TestPraznog1(s);
    TestPraznog2(s);
    return 0;
}
