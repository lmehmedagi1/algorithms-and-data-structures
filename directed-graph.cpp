#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

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
    virtual void nadjiPostojecuGranu(int &p, int &k) const = 0;
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
    
    public:
    
    GranaIterator(UsmjereniGraf<tip> *ug, bool zadnja=false) : ug(ug) {
        i=0; j=-1;
        if (zadnja) {
            i=ug->dajBrojCvorova();
            j=ug->dajBrojCvorova();
        }
        else ug->nadjiPostojecuGranu(i, j);
    }
    
    Grana<tip> operator*() {
        return ug->dajGranu(i, j);
    }
    bool operator==(const GranaIterator &iter) const {
        return (ug==iter.ug && i==iter.i && j==iter.j);
    }
    bool operator!=(const GranaIterator &iter) const {
        return !(*this==iter);
    }
    GranaIterator& operator++() {
        ug->nadjiPostojecuGranu(i, j);
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
    bool postoji;
    
    public:
    Pair(float w, tip oznaka, bool e=true) : w(w), oznaka(oznaka), postoji(e) {} 
    Pair() { postoji=false; }
    ~Pair() {}
    
    
    float getWeight() const { return w; }
    float &getWeight() { return w; }
    void setWeight(float w) { this->w=w; }
    
    tip getOznaka() const { return oznaka; }
    tip &getOznaka() { return oznaka; }
    void setOznaka(tip o) { oznaka=o; }
    
    bool postojiLi() const { return postoji; }
    void setPostoji(bool e) { postoji=e; }
    
    
    Pair &operator =(const Pair &p) {
        w=p.getWeight();
        oznaka=p.getOznaka();
        postoji=p.postojiLi();
        return *this;
    }
    
    bool operator ==(const Pair &p) {
        return (w==p.getWeight() && oznaka==p.getOznaka() && postoji==p.postojiLi());
    }
};

template <typename tip>
class MatricaGraf : public UsmjereniGraf<tip> {
    
    std::vector<std::vector<Pair<tip>>> graf;
    std::vector<tip> cvorovi;
    
    void validirajCvor(int i) const {
        if (i<0 || i>=dajBrojCvorova()) throw "Neispravan cvor";
    }
    
    void nadjiPostojecuGranu(int &p, int &k) const override {
        k++;
        int i(p), j(k);
        for (; i<dajBrojCvorova(); i++) {
            for (; j<dajBrojCvorova(); j++) {
                if (postojiGrana(i, j)) {
                    p=i;
                    k=j;
                    return;
                }
            }
            j=0;
        }
        p=dajBrojCvorova();
        k=dajBrojCvorova();
    }
    
    
    public:
    MatricaGraf(int v) {
        if (v<0) throw "Neispravan parametar";
        graf.resize(v);
        for (int i=0; i<v; i++) graf[i].resize(v);
        cvorovi.resize(v);
    }
    ~MatricaGraf() {}
    
    int dajBrojCvorova() const override {
        return graf.size();
    }
    void postaviBrojCvorova(int v) override {
        if (v<dajBrojCvorova()) throw "Greska";
        if (v==dajBrojCvorova()) return;
        graf.resize(v);
        for (int i=0; i<dajBrojCvorova(); i++) graf[i].resize(v);
        cvorovi.resize(v);
    }
    void dodajGranu(int c1, int c2, float w=0) override {
        if (!postojiGrana(c1, c2)) {
            Pair<tip> g(w, tip(), true);
            graf[c1][c2]=g;
        }
        else throw "Grana vec postoji";
    }
    void obrisiGranu(int c1, int c2) override {
        if (postojiGrana(c1, c2)) graf[c1][c2].setPostoji(false);
    }
    void postaviTezinuGrane(int c1, int c2, float w=0) override {
        if (postojiGrana(c1, c2)) graf[c1][c2].setWeight(w);
        else throw "Grana ne postoji";
    }
    float dajTezinuGrane(int c1, int c2) const override {
        if (postojiGrana(c1, c2)) return graf[c1][c2].getWeight();
        else throw "Grana ne postoji";
    }
    bool postojiGrana(int c1, int c2) const override {
        validirajCvor(c1);
        validirajCvor(c2);
        return graf[c1][c2].postojiLi();
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
        if (postojiGrana(c1, c2)) graf[c1][c2].setOznaka(o);
        else throw "Grana ne postoji";
    }
    tip dajOznakuGrane(int c1, int c2) const override {
        if (postojiGrana(c1, c2)) return graf[c1][c2].getOznaka();
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
    
    UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > bfs_obilazak;
    bfs(g, bfs_obilazak, g->dajCvor(0));
    for (int i = 0; i < bfs_obilazak.size(); i++)
      cout << bfs_obilazak[i].dajRedniBroj() << ",";
  delete g;
    
    return 0;
}