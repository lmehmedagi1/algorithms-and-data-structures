#include <iostream>
#include <stdexcept>
#include <algorithm>
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

template <typename tipKljuca, typename tipVrijednosti>
class BinStabloMapa : public Mapa<tipKljuca, tipVrijednosti> {
    
    struct Cvor {
        tipKljuca k;
        tipVrijednosti v;
        Cvor* lijevo=nullptr;
        Cvor* desno=nullptr;
        Cvor* roditelj=nullptr;
    }; 
    
    Cvor* korijen;
    int velicina;
    
    
    void Kopiraj(Cvor* &c1, Cvor* c2, Cvor* rod) {
        if (c2!=nullptr) {
            c1=new Cvor;
            c1->k=c2->k;
            c1->v=c2->v;
            c1->roditelj=rod;
            c1->lijevo=nullptr;
            c1->desno=nullptr;
            Kopiraj(c1->lijevo, c2->lijevo, c1);
            Kopiraj(c1->desno, c2->desno, c1);
        }
    }
    
    
    Cvor *traziCvor(Cvor* korijen, tipKljuca k) const {
        if (korijen==nullptr) return korijen;
        if (korijen->k==k) return korijen;
        else if (k<korijen->k) return traziCvor(korijen->lijevo, k);
        else return traziCvor(korijen->desno, k);
    }
    
    Cvor *dodaj(tipKljuca k) {
        Cvor *c=new Cvor;
        c->k=k;
        c->v=tipVrijednosti();
        c->lijevo=nullptr;
        c->desno=nullptr;
        c->roditelj=nullptr;
        
        Cvor* p=korijen, *q=nullptr;
        
        while (p!=nullptr) {
            q=p;
            if (c->k < p->k) p=p->lijevo;
            else p=p->desno;
        }
        
        if (q==nullptr) { korijen=c; velicina++;  return korijen; }
        else {
            if (c->k < q->k) q->lijevo=c;
            else q->desno=c;
            c->roditelj=q;
        }
        
        velicina++;
        return c;
    }
    
    
    public:
    
    BinStabloMapa() {
        korijen=nullptr;
        velicina=0;
    }
    
    ~BinStabloMapa() {
        obrisi(); korijen=nullptr;
    }
    
    BinStabloMapa(const BinStabloMapa &m) {
        velicina=m.velicina;
        Kopiraj(korijen, m.korijen, nullptr);
    }
    
    BinStabloMapa(BinStabloMapa &&m) {
        velicina=m.velicina; m.velicina=0;
        korijen=m.korijen; m.korijen=nullptr;
    }
    
    BinStabloMapa &operator =(const BinStabloMapa &m) {
        if (&m!=this) {
            BinStabloMapa pomocna(m);
            *this=std::move(pomocna);
        }
        return *this;
    }
    
    BinStabloMapa &operator =(BinStabloMapa &&m) {
        if (&m!=this) {
            obrisi();
            velicina=m.velicina; m.velicina=0;
            korijen=m.korijen; m.korijen=nullptr;
        }
        return *this;
    }
    
    tipVrijednosti &operator [](tipKljuca k) override {
        Cvor *c=traziCvor(korijen, k);
        
        if (c==nullptr) c=dodaj(k);
        return c->v;
    }
    
    tipVrijednosti operator [](tipKljuca k) const override {
        Cvor *c=traziCvor(korijen, k);
        
        if (c==nullptr) return tipVrijednosti();
        return c->v;
    }
    
    int brojElemenata() const override {
        return velicina;
    }
    
    void obrisi() override {
        while (korijen!=nullptr) obrisi(korijen->k);
    }
    
    void obrisi(const tipKljuca &k) override {
        Cvor* p=korijen;
        Cvor* q=nullptr;
        Cvor* tmp=nullptr;
        Cvor* pp=nullptr;
        Cvor* rp=nullptr;
        
        while (p!=nullptr) {
            if (k==p->k) break;
            q=p;
            if (k<p->k) p=p->lijevo;
            else p=p->desno;
        }
        
        if (p==nullptr) return;
        
        if (p->lijevo==nullptr) rp=p->desno;
        else if (p->desno==nullptr) rp=p->lijevo;
        else {
            pp=p;
            rp=p->lijevo;
            tmp=rp->desno;
            while (tmp!=nullptr) {
                pp=rp;
                rp=tmp;
                tmp=rp->desno;
            }
            if (pp!=p) {
                pp->desno=rp->lijevo;
                rp->lijevo=p->lijevo;
            }
            rp->desno=p->desno;
        }
        if (q==nullptr) korijen=rp;
        else if (p==q->lijevo) q->lijevo=rp;
        else q->desno=rp;
        
        p->desno=nullptr;
        p->lijevo=nullptr;
        p->roditelj=nullptr;
        
        delete p;
        velicina--;
    }
};

void Main() {
    
    NizMapa<int, int> nm;
    BinStabloMapa<int, int> bsm;
    
    int nizBrojeva[1000];
    
    for (int i=0; i<1000; i++) nizBrojeva[i]=rand()%100000;
    
    /* 
     * Vrijeme dodavanja elemenata kod nizMape ime kompleksnost O(n) jer je potrebno proći kroz cijeli niz i dodati element na kraj ukoliko ga nema 
     * Ovo vrijeme kod binStabloMapa ima kompleksnost 2log(n)=log(n^2) jer traženje elementa zahtijeva log(n) vrijeme, kao i dodavanje istog
    */
    
    clock_t time1, time2;
    time1=clock();
    for (int i=0; i<1000; i++) nm[i]=nizBrojeva[i];
    time2=clock();
    std::cout << "Vrijeme dodavanja elemenata za NizMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    for (int i=0; i<1000; i++) bsm[i]=nizBrojeva[i];
    time2=clock();
    std::cout << "Vrijeme dodavanja elemenata za BinStabloMapa je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    /* 
     * Vrijeme pristupa postojećem elementu kod nizMape ime kompleksnost O(n) jer je potrebno proći kroz cijeli niz, odnosno onoliko elemenata koliko treba do traženog
     * Ovo vrijeme kod binStabloMapa ima kompleksnost log(n)
    */
    
    time1=clock();
    for (int i=0; i<1000; i++) nm[i];
    time2=clock();
    std::cout << "Vrijeme pristupa postojećem elementu za NizMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    for (int i=0; i<1000; i++) bsm[i];
    time2=clock();
    std::cout << "Vrijeme pristupa postojećem elementu za BinStabloMapa je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    /* Binarno stablo daje bolje rezultate kod unosa većeg broja elemenata */
}


int main() {
    Main();
    return 0;
}
