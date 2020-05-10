#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

//Mapa


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
class Par {
    tip1 kljuc;
    tip2 vrijednost;
    
    public:
    Par(tip1 k, tip2 vr) {
        kljuc=k;
        vrijednost=vr;
    } 
    Par() {}
    ~Par() {}
    tip1 getKljuc() const { return kljuc; }
    tip2 getVrijednost() const { return vrijednost; }
    tip1 &getKljuc() { return kljuc; }
    tip2 &getVrijednost() { return vrijednost; }
    void setKljuc(tip1 k) { kljuc=k; }
    void setVrijednost(tip2 vr) { vrijednost=vr; }
};

template <typename tip1, typename tip2> 
bool operator <(const Par<tip1, tip2> &p1, const Par<tip1, tip2> &p2) {
    return p1.getKljuc()<p2.getKljuc();
}

template <typename tipKljuca, typename tipVrijednosti>
class HashMapaLan : public Mapa<tipKljuca, tipVrijednosti> {
    unsigned int max;
    unsigned int (*f)(tipKljuca, unsigned int)=0;
    unsigned int velicina;
    
    std::vector<std::vector<Par<tipKljuca, tipVrijednosti>>> niz;
    
    
    void brisi() {
        niz.resize(0);
        velicina=0;
    }
    
    void iniciraj() {
        max=1000;
        velicina=0;
        niz.resize(max);
    }
    
    
    unsigned int trazi(tipKljuca k, unsigned int &j) const {
        unsigned int i=f(k, max);
        
        Par<tipKljuca, tipVrijednosti> trazeni(k, tipVrijednosti());
        
        auto it = std::lower_bound(niz[i].begin(), niz[i].end(), trazeni);
        
        if (it==niz[i].end()) j=niz[i].size();
        else if (it->getKljuc()==k) j=it-niz[i].begin();
        else j=niz[i].size();
        
        return i;
    }
    
    unsigned int dodaj(tipKljuca k, unsigned int &j) {
        unsigned int i=f(k, max);
        
        Par<tipKljuca, tipVrijednosti> trazeni(k, tipVrijednosti());
        
        auto it = std::lower_bound(niz[i].begin(), niz[i].end(), trazeni);
        
        if (it==niz[i].end()) { 
            niz[i].push_back(Par<tipKljuca, tipVrijednosti>(k, tipVrijednosti())); 
            j=niz[i].size()-1;
            velicina++;
        }
        else if (it->getKljuc()==k) j=it-niz[i].begin();
        else {
            j=it-niz[i].begin();
            niz[i].insert(niz[i].begin()+j, Par<tipKljuca, tipVrijednosti>(k, tipVrijednosti()));
            velicina++;
        }
        
        return i;
    }
    
    
    public:
    
    HashMapaLan() {
        iniciraj();
    }
    
    ~HashMapaLan() {
        brisi();
    }
    
    tipVrijednosti &operator [](tipKljuca k) override {
        if (f==0) throw("Funkcija nije definisana");
        unsigned int j;
        unsigned int i=dodaj(k, j);
        return niz[i][j].getVrijednost();
    }
    
    tipVrijednosti operator [](tipKljuca k) const override {
        if (f==0) throw("Funkcija nije definisana");
        unsigned int j;
        unsigned int i=trazi(k, j);
        
        if (j==niz[i].size()) return tipVrijednosti();
        return niz[i][j].getVrijednost();
    }
    
    int brojElemenata() const override {
        return velicina;
    }
    
    void obrisi() override {
        brisi();
        iniciraj();
    }
    
    void obrisi(const tipKljuca &k) override {
        unsigned int j;
        unsigned int i=trazi(k, j);
        if (j==niz[i].size()) return;
        niz[i].erase(niz[i].begin()+j);
        velicina--;
    }
    
    void definisiHashFunkciju(unsigned int (*nova)(tipKljuca, unsigned int)) {
        f=nova;
    }
};


//Lista graf:

template <typename tip>
class Grana;
template <typename tip>
class Cvor;
template <typename tip>
class GranaIterator;

template <typename tip>
class UsmjereniGraf {
    protected: 
	UsmjereniGraf() {}
    
    public:
    UsmjereniGraf(int vel) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int v) = 0;
    virtual void dodajGranu(int c1, int c2, float w=0) = 0;
    virtual void obrisiGranu(int c1, int c2) = 0;
    virtual void postaviTezinuGrane(int c1, int c2, float w=0) = 0;
    virtual float dajTezinuGrane(int c1, int c2) const = 0;
    virtual bool postojiGrana(int c1, int c2) const = 0;
    virtual void postaviOznakuCvora(int c, tip o) = 0;
    virtual tip dajOznakuCvora(int c) const = 0;
    virtual void postaviOznakuGrane(int c1, int c2, tip o) = 0;
    virtual tip dajOznakuGrane(int c1, int c2) const = 0;
    virtual Cvor<tip> dajCvor(int c) = 0;
    virtual Grana<tip> dajGranu(int c1, int c2) = 0;
    GranaIterator<tip> dajGranePocetak() {
        return GranaIterator<tip>(this);
    }
    GranaIterator<tip> dajGraneKraj() {
        return GranaIterator<tip>(this, true);
    }
    virtual void nadjiPostojecuGranu(int &p, int &k, int &q) const = 0;
};

template <typename tip>
class Cvor {
    UsmjereniGraf<tip> *ug;
    int i;
    
    public:
    Cvor(UsmjereniGraf<tip> *ug, int i) : ug(ug), i(i) {}
    tip dajOznaku() {
        return ug->dajOznakuCvora(i);
    }
    void postaviOznaku(tip o) {
        ug->postaviOznakuCvora(i, o);
    }
    int dajRedniBroj() {
        return i;
    }
};

template <typename tip>
class Grana {
    UsmjereniGraf<tip> *ug;
    int pocetni, krajnji;
    
    public:
    
    Grana(UsmjereniGraf<tip> *ug, int c1, int c2) : ug(ug), pocetni(c1), krajnji(c2) {}
    
    float dajTezinu() {
        return ug->dajTezinuGrane(pocetni, krajnji);
    }
    void postaviTezinu(float w) {
        ug->postaviTezinuGrane(pocetni, krajnji, w);
    }
    tip dajOznaku() {
        return ug->dajOznakuGrane(pocetni, krajnji);
    }
    void postaviOznaku(tip o) {
        ug->postaviOznakuGrane(pocetni, krajnji, o);
    }
    Cvor<tip> dajPolazniCvor() {
        return ug->dajCvor(pocetni);
    }
    Cvor<tip> dajDolazniCvor() {
        return ug->dajCvor(krajnji);
    }
};

template <typename tip>
class GranaIterator {
    UsmjereniGraf<tip> *ug;
    int i, j;
    int krajnji;
    
    public:
    
    GranaIterator(UsmjereniGraf<tip> *ug, bool zadnja=false) : ug(ug) {
        i=0; j=-1;
        if (zadnja) {
            i=ug->dajBrojCvorova();
            j=ug->dajBrojCvorova();
        }
        else ug->nadjiPostojecuGranu(i, j, krajnji);
    }
    
    Grana<tip> operator*() {
        return ug->dajGranu(i, krajnji);
    }
    bool operator==(const GranaIterator &iter) const {
        return (ug==iter.ug && i==iter.i && j==iter.j);
    }
    bool operator!=(const GranaIterator &iter) const {
        return !(*this==iter);
    }
    GranaIterator& operator++() {
        ug->nadjiPostojecuGranu(i, j, krajnji);
        return *this;
    }
    GranaIterator operator++(int) {
        GranaIterator<tip> tmp(*this);
        ++(*this);
        return tmp;
    }
};

template<typename tip>
class Pair {
    float w;
    tip oznaka;
    int krajnji;
    
    public:
    Pair(float w, tip oznaka, int k) : w(w), oznaka(oznaka), krajnji(k) {} 
    Pair() {}
    ~Pair() {}
    
    
    float getWeight() const { return w; }
    float &getWeight() { return w; }
    void setWeight(float w) { this->w=w; }
    
    tip getOznaka() const { return oznaka; }
    tip &getOznaka() { return oznaka; }
    void setOznaka(tip o) { oznaka=o; }
    
    int getKrajnji() const { return krajnji; }
    int &getKrajnji() { return krajnji; }
    void setKrajnji(int i) { krajnji=i; }
    
    Pair &operator =(const Pair &p) {
        w=p.getWeight();
        oznaka=p.getOznaka();
        krajnji=p.getKrajnji();
        return *this;
    }
    
    bool operator ==(const Pair &p) {
        return (w==p.getWeight() && oznaka==p.getOznaka() && krajnji==p.krajnji);
    }
};

template <typename tip>
class ListaGraf : public UsmjereniGraf<tip> {
    
    std::vector<std::vector<Pair<tip>>> graf;
    std::vector<tip> cvorovi;
    
    void validirajCvor(int i) const {
        if (i<0 || i>=dajBrojCvorova()) throw "Neispravan cvor";
    }
    
    void nadjiPostojecuGranu(int &p, int &k, int &q) const override {
        k++;
        
        int i(p), j(k);
        for (; i<dajBrojCvorova(); i++) {
            for (; j<graf[i].size(); j++) {
                p=i;
                k=j;
                q=graf[i][j].getKrajnji();
                return;
            }
            j=0;
        }
        p=dajBrojCvorova();
        k=dajBrojCvorova();
    }
    
    int dajGranuPomocna(int c1, int c2, bool &postoji) const {
        validirajCvor(c1);
        validirajCvor(c2);
        for (int i=0; i<graf[c1].size(); i++) {
            if (graf[c1][i].getKrajnji()==c2) { postoji=true; return i; }
        }
        return 0;
    }
    
    
    public:
    ListaGraf(int v) {
        if (v<0) throw "Neispravan parametar";
        graf.resize(v);
        cvorovi.resize(v);
    }
    ~ListaGraf() {}
    
    int dajBrojCvorova() const override {
        return graf.size();
    }
    void postaviBrojCvorova(int v) override {
        if (v<dajBrojCvorova()) throw "Greska";
        if (v==dajBrojCvorova()) return;
        graf.resize(v);
        cvorovi.resize(v);
    }
    void dodajGranu(int c1, int c2, float w=0) override {
        bool postoji(false);
        dajGranuPomocna(c1, c2, postoji);
        if (!postoji) {
            Pair<tip> g(w, tip(), c2);
            graf[c1].push_back(g);
        }
        else throw "Grana vec postoji";
    }
    void obrisiGranu(int c1, int c2) override {
        bool postoji(false);
        int i=dajGranuPomocna(c1, c2, postoji);
        if (postoji) graf[c1].erase(graf[c1].begin()+i);
    }
    void postaviTezinuGrane(int c1, int c2, float w=0) override {
        bool postoji(false);
        int i=dajGranuPomocna(c1, c2, postoji);
        if (postoji) graf[c1][i].setWeight(w);
        else throw "Grana ne postoji";
    }
    float dajTezinuGrane(int c1, int c2) const override {
        bool postoji(false);
        int i=dajGranuPomocna(c1, c2, postoji);
        if (postoji) return graf[c1][i].getWeight();
        else throw "Grana ne postoji";
    }
    bool postojiGrana(int c1, int c2) const override {
        bool postoji(false);
        dajGranuPomocna(c1, c2, postoji);
        return postoji;
    }
    void postaviOznakuCvora(int c, tip o) override {
        validirajCvor(c);
        cvorovi[c]=o;
    }
    tip dajOznakuCvora(int c) const override {
        validirajCvor(c);
        return cvorovi[c];
    }
    void postaviOznakuGrane(int c1, int c2, tip o) override {
        bool postoji(false);
        int i=dajGranuPomocna(c1, c2, postoji);
        if (postoji) graf[c1][i].setOznaka(o);
        else throw "Grana ne postoji";
    }
    tip dajOznakuGrane(int c1, int c2) const override {
        bool postoji(false);
        int i=dajGranuPomocna(c1, c2, postoji);
        if (postoji) return graf[c1][i].getOznaka();
        else throw "Grana ne postoji";
    }
    Cvor<tip> dajCvor(int c) override {
        validirajCvor(c);
        return Cvor<tip>(this, c);
    }
    
    Grana<tip> dajGranu(int c1, int c2) override {
        if (postojiGrana(c1, c2)) return Grana<tip>(this, c1, c2);
        else throw "Grana ne postoji";
    }
};


//bfs i dfs

template <typename tip>
void traziPoDubini(UsmjereniGraf<tip> *g, std::vector<bool> &posjecen, Cvor<tip> c, std::vector<Cvor<tip>> &cvorovi) {
    posjecen[c.dajRedniBroj()]=true;
    cvorovi.push_back(c);
    for (int i=0; i<g->dajBrojCvorova(); i++) {
        if (!posjecen[i] && g->postojiGrana(c.dajRedniBroj(), i)) traziPoDubini(g, posjecen, g->dajCvor(i), cvorovi);
    }
}

template <typename tip>
void dfs(UsmjereniGraf<tip> *g, std::vector<Cvor<tip>> &cvorovi, Cvor<tip> c) {
    std::vector<bool> posjecen(g->dajBrojCvorova(), false);
    traziPoDubini(g, posjecen, c, cvorovi);
    for (int i=0; i<posjecen.size(); i++) {
        if (!posjecen[i]) traziPoDubini(g, posjecen, g->dajCvor(i), cvorovi);
    }
}

template <typename tip>
void traziPoSirini(UsmjereniGraf<tip> *g, std::vector<bool> &posjecen, Cvor<tip> pocetak, std::vector<Cvor<tip>> &cvorovi) {
    posjecen[pocetak.dajRedniBroj()]=true;
    std::queue<Cvor<tip>> red;
    red.push(pocetak);
    while (!red.empty()) {
        Cvor<tip> c=red.front();
        red.pop();
        cvorovi.push_back(c);
        for (int i=0; i<g->dajBrojCvorova(); i++) {
            if (posjecen[i]==false && g->postojiGrana(c.dajRedniBroj(), i)) {
                posjecen[i]=true;
                red.push(g->dajCvor(i));
            }
        }
    }
}

template <typename tip>
void bfs(UsmjereniGraf<tip> *g, std::vector<Cvor<tip>> &cvorovi, Cvor<tip> c) {
    std::vector<bool> posjecen(g->dajBrojCvorova(), false);
    traziPoSirini(g, posjecen, c, cvorovi);
    for (int i=0; i<posjecen.size(); i++) {
        if (posjecen[i]==false) traziPoSirini(g, posjecen, g->dajCvor(i), cvorovi);
    }
}


int main() {
    clock_t time1, time2;
  time1 = clock();
  UsmjereniGraf<bool> *g = new ListaGraf<bool>(100);

  for(int i=0; i<100;i++)
    for(int j=0; j<100; j++)
        if(i!=j){
            float tezina=i + (j+i)/1000.;
            g->dodajGranu(i, j, tezina);
        }
        
  g->postaviTezinuGrane(1, 2, 1.22);
  g->postaviTezinuGrane(3, 2, 3.22);
  g->postaviTezinuGrane(4, 2, 4.22);
  g->postaviTezinuGrane(2, 3, 2.33);
  
  
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
         if (g->postojiGrana(i,j))
            g->dajTezinuGrane(i, j);
    
    time2=clock(); 
    std::cout << endl <<endl <<  "Vrijeme izvrsavanja je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    delete g;
    
    return 0;
}
