#include <iostream>
#include <stdexcept>
#include <vector>

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


void pretraga (Stek<std::vector<int>> &s, int trazeni) {
    
    if (s.brojElemenata()==0) {
        std::cout << "Nema elementa";
        return;
    }
    
    if (s.vrh().size()==0) {
        std::vector<int> v(std::move(s.skini()));
        pretraga(s, trazeni);
        s.stavi(v);
    }
    else if (s.vrh()[0]>trazeni) {
        std::vector<int> v(std::move(s.skini()));
        pretraga(s, trazeni);
        s.stavi(v);
    }
    else if (s.vrh()[0]<=trazeni && s.vrh()[s.vrh().size()-1]>=trazeni) {
        int min(0), max(s.vrh().size()), mid((min+max)/2);
        
        while (min<=max) {
            if (trazeni<s.vrh()[mid]) max=mid-1;
            else if (trazeni>s.vrh()[mid]) min=mid+1;
            else {
                std::cout << mid << " " << s.brojElemenata()-1;
                return;
            }
            mid=(min+max)/2;
        }
        std::cout << "Nema elementa";
        
    }
    else std::cout << "Nema elementa";
}

//Prazan vektor:
void TestFunkcija1() {
    Stek<std::vector<int>> s;
    s.stavi({});
    
    pretraga(s, 11);
}

//Nema elementa:
void TestFunkcija2() {
    Stek<std::vector<int>> s;
    std::vector<int> v1({1, 2, 3, 4}); s.stavi(v1);
    std::vector<int> v2({6, 7}); s.stavi(v2);
    std::vector<int> v3({9, 12, 13, 14, 15}); s.stavi(v3);
    std::vector<int> v4({22}); s.stavi(v4);
    
    pretraga(s, 11);
}

//Element na kraju vektora:
void TestFunkcija3() {
    Stek<std::vector<int>> s;
    std::vector<int> v1({1, 2, 3, 4}); s.stavi(v1);
    std::vector<int> v2({6, 7}); s.stavi(v2);
    std::vector<int> v3({9, 12, 13, 14, 15}); s.stavi(v3);
    std::vector<int> v4({22}); s.stavi(v4);
    
    pretraga(s, 15);
}


int main() {
    TestFunkcija1();
    std::cout << std::endl;
    TestFunkcija2();
    std::cout << std::endl;
    TestFunkcija3();
    return 0;
}
