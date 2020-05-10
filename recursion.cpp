#include <iostream>

int fib2_0(int n, int a=0, int b=1) {
    if (n==0) return a;
    if (n==1) return b;
    return fib2_0(n-1,b,a+b);
}

int nzd(int x, int y) {
    if (y==0) return x;
    return nzd(y, x%y);
}

int main() {
    std::cout << fib2_0(5) << std::endl;
    std::cout << nzd(3,7) << std::endl;
    return 0;
}
