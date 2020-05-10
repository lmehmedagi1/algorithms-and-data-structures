#include <iostream>
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
    bool del;
    
    public:
    Pair(tip1 k, tip2 vr) {
        kljuc=k;
        vrijednost=vr;
        del=false;
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
        del=p.del;
        return *this;
    }
    bool isDel() { return del; }
    void setDel(bool d) { del=d; }
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
        
        Cvor* p=korijen, *q=nullptr;
        
        while (p!=nullptr) {
            if (k==p->k) return p;
            q=p;
            if (k < p->k) p=p->lijevo;
            else p=p->desno;
        }
        
        Cvor *c=new Cvor;
        c->k=k;
        c->v=tipVrijednosti();
        c->lijevo=nullptr;
        c->desno=nullptr;
        c->roditelj=nullptr;
        
        
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
        Cvor *c=dodaj(k);
        
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

template <typename tipKljuca, typename tipVrijednosti>
class HashMapa : public Mapa<tipKljuca, tipVrijednosti> {
    unsigned int max;
    unsigned int (*f)(tipKljuca, unsigned int)=0;
    unsigned int velicina;
    
    Pair<tipKljuca, tipVrijednosti> *niz;
    
    void brisi() {
        delete[] niz;
        niz=nullptr;
        velicina=0;
        max=1000;
    }
    
    void iniciraj(unsigned int vel, Pair<tipKljuca, tipVrijednosti>* &p) {
        p=new Pair<tipKljuca, tipVrijednosti>[max];
        for (unsigned int i=0; i<vel; i++) { p[i].setVrijednost(tipVrijednosti()); p[i].setKljuc(tipKljuca()); p[i].setDel(false); } 
    }
    
    void kopiraj(Pair<tipKljuca, tipVrijednosti>* &p1, Pair<tipKljuca, tipVrijednosti> *p2) {
        for (unsigned int i=0; i<max; i++) {
            p1[i]=p2[i];
        }
    }
    
    unsigned int trazi(tipKljuca k) const {
        unsigned int i=f(k, max);
        
        if (niz[i].getKljuc()==k && niz[i].isDel()==false) return i;
        
        unsigned int j=i+1;
        while((niz[j%max].isDel() || niz[j%max].getVrijednost()!=tipVrijednosti()) && j%max!=i) {
            if (niz[j%max].getKljuc()==k && niz[j%max].isDel()==false) return j%max;  
            j++;
        } 
        
        return -1;
    }
    
    void prosiri() {
        Pair<tipKljuca, tipVrijednosti> *novi=new Pair<tipKljuca, tipVrijednosti>[max*2];
        iniciraj(max*2, novi);
        kopiraj(novi, niz);
        delete[] niz;
        niz=novi;
        delete[] novi;
        max=max*2;
    }
    
    unsigned int dodaj(tipKljuca k) {
        
        if (velicina==max) prosiri();
        
        unsigned int i=f(k, max);
        
        if (niz[i].getVrijednost()==tipVrijednosti() || niz[i].isDel()) { niz[i].setVrijednost(tipVrijednosti()); niz[i].setKljuc(k); velicina++; return i; }
        
        unsigned int j;
        
        for (j=i+1; j%max!=i; j++) {
            if (niz[j%max].isDel() || niz[j%max].getVrijednost()==tipVrijednosti()) {
                niz[j%max].setVrijednost(tipVrijednosti());
                niz[j%max].setKljuc(k);
                velicina++;
                break;
            }
        }
        return j%max;
    }
    
    
    public:
    
    HashMapa() {
        max=1000;
        velicina=0;
        iniciraj(max, niz);
    }
    
    ~HashMapa() {
        brisi();
    }
    
    HashMapa(const HashMapa &m) {
        velicina=m.velicina;
        max=m.max;
        iniciraj(max, niz);
        kopiraj(niz, m.niz);
        f=m.f;
    }
    
    HashMapa(HashMapa &&m) {
        velicina=m.velicina; m.velicina=0;
        max=m.max;
        niz=m.niz; m.niz=nullptr;
        f=m.f;
    }
    
    HashMapa &operator =(const HashMapa &m) {
        if (&m!=this) {
            HashMapa pomocna(m);
            *this=std::move(pomocna);
        }
        return *this;
    }
    
    HashMapa &operator =(HashMapa &&m) {
        if (&m!=this) {
            brisi();
            velicina=m.velicina; m.velicina=0;
            max=m.max;
            niz=m.niz; m.niz=nullptr;
            f=m.f;
        }
        return *this;
    }
    
    
    
    tipVrijednosti &operator [](tipKljuca k) override {
        if (f==0) throw("Funkcija nije definisana");
        int i=trazi(k);
        
        if (i==-1) i=dodaj(k);
        return niz[i].getVrijednost();
    }
    
    tipVrijednosti operator [](tipKljuca k) const override {
        if (f==0) throw("Funkcija nije definisana");
        int i=trazi(k);
        
        if (i==-1) return tipVrijednosti();
        return niz[i].getVrijednost();
    }
    
    int brojElemenata() const override {
        return velicina;
    }
    
    void obrisi() override {
        brisi();
        iniciraj(max, niz);
    }
    
    void obrisi(const tipKljuca &k) override {
        unsigned int i=trazi(k);
        if (i==-1) return;
        niz[i].setDel(true);
        velicina--;
    }
    
    
    void definisiHashFunkciju(unsigned int (*nova)(tipKljuca, unsigned int)) {
        f=nova;
    }
};


void Main() {
    
    NizMapa<int, int> nm;
    BinStabloMapa<int, int> bsm;
    HashMapa<int, int> hm;
    auto f=[](int x, unsigned int i) { return (unsigned int)(x%i); };
    hm.definisiHashFunkciju(f);
    
    int nizBrojeva[1000];
    
    for (int i=0; i<1000; i++) nizBrojeva[i]=rand()%100000;
    
    /* 
     * Vrijeme dodavanja elemenata kod nizMape ime kompleksnost O(n) jer je potrebno proći kroz cijeli niz i dodati element na kraj ukoliko ga nema 
     * Ovo vrijeme kod binStabloMapa ima kompleksnost O(log(n)) jer prvo traži njegovu poziciju
     * Ovo vrijeme kod HashMapa ima kompleksnost O(1) jer ne zavisi od broja elemenata u nizu, u slučaju kolizije ovo vrijeme je nešto duže, ovisno o načinu na koji je rješavamo
     * Linearno ispitivanje donosi najgore rezultate jer se element može nalaziti bilo gdje u nizu, a moramo provjeriti da li je prisutan
    */
    
    clock_t time1, time2;
    time1=clock();
    for (int i=0; i<1000; i++) nm[nizBrojeva[i]]=nizBrojeva[i];
    time2=clock();
    std::cout << "Vrijeme dodavanja elemenata za NizMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    for (int i=0; i<1000; i++) bsm[nizBrojeva[i]]=nizBrojeva[i];
    time2=clock();
    std::cout << "Vrijeme dodavanja elemenata za BinStabloMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    for (int i=0; i<1000; i++) hm[nizBrojeva[i]]=nizBrojeva[i];
    time2=clock();
    std::cout << "Vrijeme dodavanja elemenata za HashMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    /* 
     * Vrijeme pristupa postojećem elementu kod nizMape ime kompleksnost O(n) jer je potrebno proći kroz cijeli niz, odnosno onoliko elemenata koliko treba do traženog
     * Ovo vrijeme kod binStabloMapa ima kompleksnost O(log(n))
     * Ovo vrijeme kod HashMapa ima kompleksnost O(1) jer ne zavisi od broja elemenata u nizu, u slučaju kolizije ovo vrijeme je nešto duže, ovisno o načinu na koji je rješavamo
     * Linearno ispitivanje donosi najgore rezultate jer se element može nalaziti bilo gdje u nizu
    */
    
    time1=clock();
    for (int i=0; i<1000; i++) nm[nizBrojeva[i]];
    time2=clock();
    std::cout << "Vrijeme pristupa postojećem elementu za NizMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    for (int i=0; i<1000; i++) bsm[nizBrojeva[i]];
    time2=clock();
    std::cout << "Vrijeme pristupa postojećem elementu za BinStabloMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    for (int i=0; i<1000; i++) hm[nizBrojeva[i]];
    time2=clock();
    std::cout << "Vrijeme pristupa postojećem elementu za HashMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    /*
     * Vrijeme brisanja elemenata kod nizMape ima kompleksnost O(n) jer se prolazi kroz čitav niz da se nađe element, zatim se svi elementi nakon njega pomjeraju za jedno mjesto 
     * Ovo vrijeme kod BinStabloMape ima kompleksnost O(log(n)) gdje je h visina stabla, jer je potrebno preurediti stablo kada dođe do brisanja
     * Kod HashMape ovo vrijeme ima kompleksnost O(1) jer na mjesto obrisanog elementa postavljamo oznaku DEl
    */
    
    
    time1=clock();
    nm.obrisi();
    time2=clock();
    std::cout << "Vrijeme brisanja elemenata za NizMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    bsm.obrisi();
    time2=clock();
    std::cout << "Vrijeme brisanja elemenata za BinStabloMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    hm.obrisi();
    time2=clock();
    std::cout << "Vrijeme brisanja elemenata za HashMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    /* Za veći broj elemenata u sva tri slučaja najbolje je izabrati prikladno dizajniranu heš funkciju */
}


int main() {
    
    Main();
   
    return 0;
}
