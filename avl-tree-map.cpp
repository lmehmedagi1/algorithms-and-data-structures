#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <iomanip>

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
    //virtual void ispisi() =0;
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
        
        //Prvo provjeravamo nalazi li se u stablu, odnosno tražimo mu roditelja
        while (p!=nullptr) {
            q=p;
            if (p->k==k) return p;
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
class AVLStabloMapa : public Mapa<tipKljuca, tipVrijednosti> {
    
    struct Cvor {
        tipKljuca k;
        tipVrijednosti v;
        Cvor* lijevo=nullptr;
        Cvor* desno=nullptr;
        Cvor* roditelj=nullptr;
        //Novi argument
        int balans;
    }; 
    
    Cvor* korijen;
    int velicina;
    
    //Dodana funkcija radi bržeg brisanja cijelog stabla, bez ažuriranja balansa
    void brisiStablo(Cvor* &c) {
        if (c!=nullptr) {
            brisiStablo(c->lijevo);
            brisiStablo(c->desno);
            c->roditelj=nullptr;
            c->lijevo=nullptr;
            c->desno=nullptr;
            delete c; c=nullptr;
        }
        velicina=0;
    }
    
    
    void Kopiraj(Cvor* &c1, Cvor* c2, Cvor* rod) {
        if (c2!=nullptr) {
            c1=new Cvor;
            c1->k=c2->k;
            c1->v=c2->v;
            c1->roditelj=rod;
            c1->lijevo=nullptr;
            c1->desno=nullptr;
            //Moramo kopirati i balans
            c1->balans=c2->balans;
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
            if (p->k==k) return p;
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
        c->balans=0;
        
        
        if (q==nullptr) { korijen=c; velicina++;  return korijen; }
        else {
            if (c->k < q->k) q->lijevo=c;
            else q->desno=c;
            c->roditelj=q;
        }
        
        //Provjeravamo balans:
        if (q->desno && q->lijevo) q->balans=0;
        else if (q->desno) azurirajBalans(q, true);
        else azurirajBalans(q, false);
        
        
        velicina++;
        return c;
    }
    
    
    Cvor *LijevaRotacija(Cvor* c) {
        Cvor* x=c->desno;
        
        Cvor* y=x->lijevo; 
        
        x->lijevo=c;
        x->roditelj=c->roditelj;
        if (x->roditelj==nullptr) korijen=x;
        else if (c==c->roditelj->desno) x->roditelj->desno=x;
        else x->roditelj->lijevo=x;
        
        c->roditelj=x;
        
        c->desno=nullptr;
        
        c->desno=y; 
        if (y!=nullptr) y->roditelj=c;
        
        c->balans=0;
        x->balans=0;
        
        return c;
    }
    
    Cvor *DesnaRotacija(Cvor* c) {
        Cvor* x=c->lijevo;
        
        Cvor* y=x->desno; 
        
        x->desno=c;
        x->roditelj=c->roditelj;
        if (x->roditelj==nullptr) korijen=x;
        else if (c==c->roditelj->desno) x->roditelj->desno=x;
        else x->roditelj->lijevo=x;
        
        c->roditelj=x;
        
        c->lijevo=nullptr;
        
        c->lijevo=y;   
        if (y!=nullptr) y->roditelj=c; 
        
        c->balans=0;
        x->balans=0;
        
        return c;
    }
    
    Cvor *DesnaLijevaRotacija(Cvor* t) {
        DesnaRotacija(t->desno);
        return LijevaRotacija(t);
    }
    
    Cvor *LijevaDesnaRotacija(Cvor* t) {
        LijevaRotacija(t->lijevo);
        return DesnaRotacija(t);
    } 
    
    void azurirajBalans(Cvor* c, bool desno) {
        if (c==nullptr) return;
        
        if (desno) c->balans--;
        else c->balans++;
        
        if(c->balans < -1) {
		    if(c->desno->balans == -1)
			    LijevaRotacija(c);
		    else {
		        DesnaLijevaRotacija(c);
		    }
	    } 
	    else if(c->balans > 1) {
		    if(c->lijevo->balans == 1) {
			    DesnaRotacija(c);
		    } else {
		        LijevaDesnaRotacija(c);
		    }
	    } 
	    else if (c->balans==0) return;
	    else if(c->roditelj!=nullptr) {
	        if(c == c->roditelj->lijevo) azurirajBalans(c->roditelj, false);
		   	else azurirajBalans(c->roditelj, true);
	    }   
    }
    
    /*
    void azurirajBalansBrisanje(Cvor* c, bool desno) {
        if (c==nullptr) return;
        
        if (desno) c->balans++;
        else c->balans--;
        
        if(c->balans < -1) {
		    if(c->desno->balans == -1)
			    LijevaRotacija(c);
		    else {
		        DesnaLijevaRotacija(c);
		    }
	    } 
	    else if(c->balans > 1) {
		    if(c->lijevo->balans == 1) {
			    DesnaRotacija(c);
		    } else {
		        LijevaDesnaRotacija(c);
		    }
	    }
	    else if (c->balans==1) return;
	    else if(c->roditelj!=nullptr) {
	        if(c == c->roditelj->lijevo) azurirajBalansBrisanje(c->roditelj, false);
		   	else azurirajBalansBrisanje(c->roditelj, true);
	    }   
    }
    */
    
    public:
    
    AVLStabloMapa() {
        korijen=nullptr;
        velicina=0;
    }
    
    ~AVLStabloMapa() {
        obrisi(); korijen=nullptr;
    }
    
    AVLStabloMapa(const AVLStabloMapa &m) {
        velicina=m.velicina;
        Kopiraj(korijen, m.korijen, nullptr);
    }
    
    AVLStabloMapa(AVLStabloMapa &&m) {
        velicina=m.velicina; m.velicina=0;
        korijen=m.korijen; m.korijen=nullptr;
    }
    
    AVLStabloMapa &operator =(const AVLStabloMapa &m) {
        if (&m!=this) {
            AVLStabloMapa pomocna(m);
            *this=std::move(pomocna);
        }
        return *this;
    }
    
    AVLStabloMapa &operator =(AVLStabloMapa &&m) {
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
        brisiStablo(korijen);
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
        
        if (p->lijevo==nullptr) { rp=p->desno; pp=q; }
        else if (p->desno==nullptr) { rp=p->lijevo; pp=q; }
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
                if (rp->lijevo) rp->lijevo->roditelj=pp;
                rp->lijevo=p->lijevo;
                if (p->lijevo) p->lijevo->roditelj=rp;
            }
            rp->desno=p->desno;
            if (p->desno) p->desno->roditelj=rp;
        }
        
        if (q==nullptr) korijen=rp;
        else if (p==q->lijevo) { q->lijevo=rp; q->balans--; }
        else { q->desno=rp; q->balans++; }
        
        
        if (rp!=nullptr) rp->roditelj=q;
        
        /*
        //Azuriramo balans 
        if (pp!=nullptr) {
            pp->balans++;
            if (pp->balans!=1 && pp->roditelj!=nullptr) {
                if (pp->roditelj->desno==pp) azurirajBalansBrisanje(pp->roditelj, true);
                else azurirajBalansBrisanje(pp->roditelj, false);
            }
        }
        */
        
        p->desno=nullptr;
        p->lijevo=nullptr;
        p->roditelj=nullptr;
        
        delete p; p=nullptr;
        velicina--;
    }
    
    
    /*
    void ispisi(Cvor* p)  {
        if (p!=nullptr) {

        cout << "Posmatram " << p->k;
        if (p==korijen) cout << " to je korijen ";
        if (p->lijevo) cout << " lijevo dijete mu je " << p->lijevo->k;
        if (p->desno) cout << " a desno dijete mu je " << p->desno->k;
        if (p->roditelj) cout << " a roditelj " << p->roditelj->k;
        cout << " balans mu je " << p->balans;
        cout << endl; 
        
        ispisi(p->lijevo);
        ispisi(p->desno);
        
        }
    }
    Cvor* getKorijen() { return korijen; }
    */
};

void Main() {
    
    AVLStabloMapa<int, int> avlsm;
    BinStabloMapa<int, int> bsm;
    
    int nizBrojeva[5000];
    
    for (int i=0; i<5000; i++) nizBrojeva[i]=rand()%100000;
    
    /* 
     * Vrijeme dodavanja elemenata kod ove dvije vrste stabala ima kompleksnost O(log(n)) jer prvo mora pronaći njegovu poziciju
     * Kod AVLStabloMape ova operacija je nešto kompleksnija jer se ažurira balans stabla
    */
    
    clock_t time1, time2;
    
    
    time1=clock();
    for (int i=0; i<5000; i++) bsm[nizBrojeva[i]]=i;
    time2=clock();
    std::cout << "Vrijeme dodavanja elemenata za BinStabloMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    for (int i=0; i<5000; i++) avlsm[nizBrojeva[i]]=i;
    time2=clock();
    std::cout << "Vrijeme dodavanja elemenata za AVLStabloMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    /* 
     * Vrijeme pristupa postojećem elementu kod ove dvije vrste stabala je jednaka i iznosi O(h) gdje je h visina stabla
    */
    
    time1=clock();
    for (int i=0; i<5000; i++) bsm[nizBrojeva[i]];
    time2=clock();
    std::cout << "Vrijeme pristupa postojećem elementu za BinStabloMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    for (int i=0; i<5000; i++) avlsm[nizBrojeva[i]];
    time2=clock();
    std::cout << "Vrijeme pristupa postojećem elementu za AVLStabloMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    /* 
     * Vrijeme brisanja elemenata kod ove dvije vrste stabala ima kompleksnost O(log(n)) jer prvo mora pronaći njegovu poziciju, a zatim i preurediti stablo
    */
    
    
    time1=clock();
    for (int i=0; i<5000; i++) bsm.obrisi(nizBrojeva[i]);
    time2=clock();
    std::cout << "Vrijeme brisanja elemenata za BinStabloMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    
    time1=clock();
    for (int i=0; i<5000; i++) avlsm.obrisi(nizBrojeva[i]);
    time2=clock();
    std::cout << "Vrijeme brisanja elemenata za AVLStabloMapu je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    /* Zbog svoje uređenost, AVL stabla su efikasnija za veći broj elemenata */
}



int main() {
    Main();
    
    
    
    return 0;
}
