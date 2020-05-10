#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void sortPoCiframa(vector<int> &a, int k) {
        int brojac[10]={0};        
        int n(a.size());
        
        vector<int> izlaz(n);
        
        for(int i=0; i<n; i++) brojac[(a[i]/k)%10]++;
        for(int i=1; i<10; i++) brojac[i]+=brojac[i-1];
        for(int i=n-1; i>=0; i--) {
                izlaz[brojac[(a[i]/k)%10]-1]=a[i];
                brojac[(a[i]/k)%10]--;
        }
        
        for(int i=0; i<n; i++) a[i]=izlaz[i];
}

void radixSort(vector<int> &a) {
    int max=std::abs(*std::max_element(a.begin(), a.end()));
    
    int k=1;
    while(max) {
            sortPoCiframa(a,k);
            k*=10;
            max/=10;
    }
}

bool jeLiList(vector<int> &a, int i, int &velicina) {
    return (i>=velicina/2 && i<velicina);
}

int lijevoDijete(int i) {
    return 2*i+1;
}

int desnoDijete(int i) {
    return 2*i+2;
}

int roditelj(int i) {
    return (i-1)/2;
}

void popraviDolje(vector<int> &a, int i, int &velicina) {
    
    while(!jeLiList(a, i, velicina)) {
        
        int veci(lijevoDijete(i));
        int dd(desnoDijete(i));
        
        if (dd<velicina && a[dd]>a[veci]) veci=dd;
        if (a[i]>a[veci]) return;
        
        int temp(a[i]);
        a[i]=a[veci];
        a[veci]=temp;
        
        i=veci;
    }
}

void popraviGore(vector<int> &a, int i) {
    while (i!=0 && a[i]>a[roditelj(i)]) {
        int temp(a[i]);
        a[i]=a[roditelj(i)];
        a[roditelj(i)]=temp;
        i=roditelj(i);
    }
}

void stvoriGomilu(vector<int> &a) {
    int velicina(a.size());
    for (int i=velicina/2; i>=0; i--) popraviDolje(a, i, velicina);
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a, velicina-1);
}

int izbaciPrvi(vector<int> &a, int &velicina) {
    if (velicina==0) throw "Gomila je prazna";
    velicina--;
    
    int temp(a[0]);
    a[0]=a[velicina];
    a[velicina]=temp;
    
    if (velicina!=0) popraviDolje(a, 0, velicina);
    return a[velicina];
}

void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    int velicina(a.size());
    for (int i=0; i<a.size()-1; i++) {
        izbaciPrvi(a, velicina);
    }
}

int main() {
    std::vector<int> v={ 213, 22, 657, 23, 444, 224 };
    gomilaSort(v);
    std::for_each(v.begin(), v.end(), [](int x){ std::cout << x << " "; } );
    return 0;
}
