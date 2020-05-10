#include <iostream>
#include <fstream>
#include <string>

template <typename Tip>
void bubble_sort(Tip* niz, int vel) {
    for (int i=vel-1; i>=1; i--) {
        for (int j=1; j<=i; j++) {
            if (niz[j-1]>niz[j]) {
                Tip tmp=niz[j-1];
                niz[j-1]=niz[j];
                niz[j]=tmp;
            }
        }
    }
}

template <typename Tip>
void selection_sort(Tip* niz, int vel) {
    for (int i=0; i<vel-1; i++) {
        Tip min=niz[i];
        int indmin=i;
        for (int j=i+1; j<vel; j++) {
            if (niz[j]<min) {
                min=niz[j];
                indmin=j;
            }
        }
        niz[indmin]=niz[i];
        niz[i]=min;
    }
}

template <typename Tip>
void Zamjeni(Tip* niz, const int i, const int j) {
    Tip tmp(niz[i]);
    niz[i]=niz[j];
    niz[j]=tmp;
} 

template <typename Tip>
int Particija(Tip* niz, const int prvi, const int zadnji) {
    Tip pivot(niz[prvi]);
    int p=prvi+1;
    while (p<=zadnji) {
        if (niz[p]<=pivot) p++;
        else break;
    }
    for (int i=p+1; i<=zadnji; i++) {
        if (niz[i]<pivot) {
            Zamjeni(niz, p, i);
            p++;
        }
    }
    Zamjeni(niz, prvi, p-1);
    return p-1;
}

template <typename Tip>
void Quick(Tip* niz, const int prvi, const int zadnji) {
    if (prvi<zadnji) {
        int j=Particija(niz, prvi, zadnji);
        Quick(niz, prvi, j-1);
        Quick(niz, j+1, zadnji);
    }
}

template <typename Tip>
void quick_sort(Tip* niz, int vel) {
    Quick<Tip>(niz, 0, vel-1);
}

template <typename Tip>
void Merge(Tip* niz, const int l, const int p, const int q, const int u) {
    int i(0), j(q-l), k(l);
    Tip* B= new Tip[u-l+1];
    for (int m=0; m<=u-l; m++) B[m]=niz[l+m];
    while (i<=p-l && j<=u-l) {
        if (B[i]<B[j]) niz[k]=B[i++];
        else niz[k]=B[j++];
        k++;
    }
    while (i<=p-l) niz[k++]=B[i++];
    while (j<=u-l) niz[k++]=B[j++];
    delete[] B;
}

template <typename Tip>
void MergeSort(Tip* niz, const int l, const int u) {
    if (u > l) {
        int p = (l + u - 1) / 2;
        int q = p + 1;
        MergeSort(niz, l, p);
        MergeSort(niz, q, u);
        Merge(niz, l, p, q, u);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel) {
    MergeSort(niz, 0, vel-1);
}

void ucitaj(std::string filename, int*& niz, int &vel) {
    std::ifstream ulazni_tok(filename);
    if (!ulazni_tok) throw "Ucitavanje biblioteke nije uspjelo";
    
    int el;
    int brojac(0);
    
    while (ulazni_tok >> el) brojac++;
    
    if (ulazni_tok.eof()) vel=brojac;
    else throw "Greska pri citanju iz biblioteke";
    
    niz=new int[vel];
    
    ulazni_tok.close(); 
    ulazni_tok.open(filename);
    
    for (int i=0; i<vel; i++) {
        ulazni_tok >> el;
        niz[i]=el;
    }
}

void generisi(std::string filename, int vel) {
    std::ofstream izlazni_tok(filename);
    srand(time(NULL));
    for (int i=0; i<vel; i++) {
        izlazni_tok << rand()%100;
        if (i<vel-1) izlazni_tok << ' ';
    }
}

void ProvjeriJeLiSortiran(int* p, int vel) {
    for (int i=1; i<vel; i++) {
        if (p[i]<p[i-1]) { std::cout << "Niz nije sortiran" << std::endl; return; }
    }
    std::cout << "Niz je sortiran" << std::endl;
}

void Zapisi(std::string filename, int* p, int vel) {
    std::ofstream izlazni_tok(filename);
    for (int i=0; i<vel; i++) {
        izlazni_tok << p[i];
        if (i<vel-1) izlazni_tok << ' ';
    }
}

void Main() {
    
    clock_t time1, time2;
    time1=clock();
    
    int izbor;
    std::string biblioteka;
    std::cout << "Izaberite nacin unosa: (1 - slucajni brojevi, 2 - podaci iz vase biblioteke): " << std::endl;
    std::cin >> izbor;
    if (izbor==1) {
        biblioteka="test.txt";
        generisi(biblioteka, 100);
    }
    else if (izbor==2) {
        std::cout << "Unesite ime biblioteke: " << std::endl;
        std::cin >> biblioteka;
    }
    else { std::cout << "Pogresan izbor!"; return; }
    
    std::cout << "Izaberite nacin unosa: (1 - Bubble sort, 2 - Selection sort, 3 - Quick sort, 4 - Merge sort): " << std::endl;
    std::cin >> izbor;
    
    int* p;
    int vel(0);
    ucitaj(biblioteka, p, vel);
    
    if (izbor==1) bubble_sort(p, vel);
    else if (izbor==2) selection_sort(p, vel);
    else if (izbor==3) quick_sort(p, vel);
    else if (izbor==4) merge_sort(p, vel);
    else { std::cout << "Pogresan izbor!"; return; }
    
    ProvjeriJeLiSortiran(p, vel);
    
    std::cout << "Unesite naziv biblioteke u koju upisuje sortiran niz: " << std::endl;
    std::cin.ignore(1000, '\n');
    std::getline(std::cin, biblioteka);
    
    Zapisi(biblioteka, p, vel);
    
    time2=clock();
    std::cout << "Vrijeme izvrsenja funkcije je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms";
    delete[] p;
}


int main() {
    Main();
    return 0;
}
