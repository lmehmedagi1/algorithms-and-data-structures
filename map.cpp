#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
    
    public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual TipVrijednosti &operator [](TipKljuca k) = 0;
    virtual TipVrijednosti operator [](TipKljuca k) const =0;
    virtual int brojElemenata() const  = 0;
    virtual void obrisi() =0;
    virtual void obrisi(const TipKljuca &k) =0;
};


template<typename tip1, typename tip2>
class Pair {
    tip1 kljuc;
    tip2 vrijednost;
    
    public:
    Pair(tip1 k, tip2 vr) {
        kljuc=k;
        vrijednost=vr;
    } 
    Pair() {}
    ~Pair() {}
    tip1 getKljuc() const { return kljuc; }
    tip2 getVrijednost() const { return vrijednost; }
    tip1 &getKljuc() { return kljuc; }
    tip2 &getVrijednost() { return vrijednost; }
    void setKljuc(tip1 k) { kljuc=k; }
    void setVrijednost(tip2 vr) { vrijednost=vr; }
    Pair &operator =(const Pair &p) {
        kljuc=p.getKljuc();
        vrijednost=p.getVrijednost();
        return *this;
    }
};

template<typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
    
    Pair<TipKljuca, TipVrijednosti> *niz;
    int kapacitet;
    int velicina;
    
    void dodaj(const TipKljuca &k) {
        if (velicina==kapacitet) {
            kapacitet*=2;
            Pair<TipKljuca, TipVrijednosti> *p=new Pair<TipKljuca, TipVrijednosti>[kapacitet]{};
            for (int i=0; i<velicina; i++) {
                p[i]=niz[i];
            }
            delete[] niz;
            niz=p; p=nullptr;
        }
        niz[velicina]=Pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti());
        velicina++;
    }
    
    public:
    NizMapa() {
        niz=new Pair<TipKljuca, TipVrijednosti>[50];
        kapacitet=50;
        velicina=0;
    }
    
    ~NizMapa() {
        if (niz!=nullptr) { delete[] niz; niz=nullptr; }
    }
    
    NizMapa(const NizMapa &m) {
        kapacitet=m.kapacitet;
        velicina=m.velicina;
        niz=new Pair<TipKljuca, TipVrijednosti>[kapacitet];
        for (int i=0; i<velicina; i++) {
            niz[i]=m.niz[i];
        }
    }
    
    NizMapa(NizMapa &&m) {
        niz=m.niz; m.niz=nullptr;
        velicina=m.velicina; m.velicina=0;
        kapacitet=m.kapacitet; m.kapacitet=0;
    }
    
    NizMapa &operator =(const NizMapa &m) {
        if (&m!=this) {
            NizMapa pomocna(m);
            *this=std::move(pomocna);
        }
        return *this;
    }
    
    NizMapa &operator =(NizMapa &&m) {
        if (&m!=this) {
            delete[] niz;
            niz=m.niz; m.niz=nullptr;
            velicina=m.velicina; m.velicina=0;
            kapacitet=m.kapacitet; m.kapacitet=0;
        }
        return *this;
    }
    
    TipVrijednosti &operator [](TipKljuca k) override {
        
        for (int i=0; i<velicina;  i++) {
            if (niz[i].getKljuc()==k) return niz[i].getVrijednost();
        }
        
        dodaj(k);
        return niz[velicina-1].getVrijednost();
    }
    
    TipVrijednosti operator [](TipKljuca k) const override {
        for (int i=0; i<velicina;  i++) {
            if (niz[i].getKljuc()==k) return niz[i].getVrijednost();
        }
        return TipVrijednosti();
    } 
    
    int brojElemenata() const override {
        return velicina;
    } 
    
    void obrisi() override {
        delete[] niz;
        niz=new Pair<TipKljuca, TipVrijednosti>[50];
        kapacitet=50;
        velicina=0;
    }
    
    void obrisi(const TipKljuca &k) override {
        for (int i=0; i<velicina; i++) {
            if (niz[i].getKljuc()==k) {
                for (int j=i; j<velicina-1; j++) {
                    niz[j]=niz[j+1];
                }
                velicina--;
                return;
            }
        }
    }
    
};


void testOperatora() {
    NizMapa<int, int> m;
    m[1]=2;
    m[2]=4;
    m[3]=8;
    m[4]=16;
    
    if (m[2]==4 && m[5]==0) std::cout << "Test operatora []: OK" << std::endl;
    else std::cout << "Test operatora []: Greska" << std::endl;
}
void testBrojElemenata() {
    NizMapa<std::string,std::string> m1;
    m1["Sarajevo"] = "BiH";
    m1["Oslo"] = "Norveska";
    m1["Amsterdam"] = "Holandija";
    m1["Oslo"]="Norveska2";
    
    if (m1.brojElemenata()==3) std::cout << "Test broja elemenata: OK" << std::endl;
    else std::cout << "Test broja elemenata: Greska" << std::endl;
}
void testObrisi() {
    NizMapa<int, int> m;
    m[1]=2;
    m[2]=4;
    m[3]=8;
    m[4]=16;
    m.obrisi();
    m[5]=32;
    if (m.brojElemenata()==1) std::cout << "Test brisanja mape: OK" << std::endl;
    else std::cout << "Test brisanja mape: Greska" << std::endl;
}
void testKopiranja() {
    NizMapa<std::string,std::string> m1;
    m1["Sarajevo"] = "BiH";
    m1["Oslo"] = "Norveska";
    m1["Amsterdam"] = "Holandija";
    
    NizMapa<std::string,std::string> m2(m1);
    m1.obrisi("Oslo");
    
    if (m1.brojElemenata()==2 && m2.brojElemenata()==3) std::cout << "Test kopirajuceg konstruktora: OK" << std::endl;
    else std::cout << "Test kopirajuceg konstruktora: Greska" << std::endl;
}
void testDodjele() {
    NizMapa<std::string,std::string> m1;
    m1["Sarajevo"] = "BiH";
    m1["Oslo"] = "Norveska";
    m1["Amsterdam"] = "Holandija";
    
    NizMapa<std::string,std::string> m2;
    m2["London"] = "UK";
    
    m2=m1;
    
    if (m2.brojElemenata()==3) std::cout << "Test operatora dodjele: OK" << std::endl;
    else std::cout << "Test operatora dodjele: Greska" << std::endl;
}


int main() {

    testBrojElemenata();
    testOperatora();
    testObrisi();
    testKopiranja();
    testDodjele();
    
    return 0;
}
