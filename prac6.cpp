#include <iostream>

using namespace std;
char c;

int A();
int C();

int E() {
    c = getchar();
    if(c != 'a' && c != '\n')
        return 1;
    return c == 'a' ? A() : 0;
}

int A() {
    c = getchar();
    if(c != 'a')
        return 1;
    return E();
}

int B() {
    c = getchar();
    if(c != 'a')
        return 1;
    return A();
}

int F() {
    c = getchar();
    if(c != 'a' && c != '\n')
        return 1;
    return c == 'a' ? C() : 0;
}

int C() {
    c = getchar();
    if(c != 'a' && c != 'b')
        return 1;
    return c == 'a' ? F() : B();
}

int D() {
    c = getchar();
    if(c != 'a')
        return 1;
    return C();
}

int H() {
    c = getchar();
    if(c != 'b')
        return 1;
    return D();
}

int main() {
    cout << (!H() ? "yes" : "no") << endl;
}