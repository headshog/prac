#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

/*class A {
private:
    int a;
    static int cnt;
    static A obj;
    A(int i = 0) : a(i) {}
    A(const A& el) {
        a = el.a;
    }
    ~A() {}
public:
    static A& cr(int i = 0) {
        if(!cnt) {
            cnt++;
            obj.a = i;
        }
        else if(cnt == 1) {
            obj.a = i;
            cnt++;            
        }
        return obj;
    }
    static A* cp(const A& el) {
        return &obj;
    }
    void print() const {
        cout << a << endl;
    }
};
*/
/*
class A {
private:
    int a;
    ~A() {}
public:
    A(int i = 0) : a(i) {}
    A(const A& el) {
        a = el.a;
    }
    static void del(const A& el) {
        el.~A();
    }
};

int main() {
    char* buf = new char[10000000];
    A* ptr[100];
    for(int i=0;i<100;i++) {
        ptr[i] = new (&buf + i*sizeof(A)) A;
    }
    for(int i=0;i<100;i++) {
        A::del(*(ptr[i]));
    }
    delete[] buf;
}
*/

class A {
private:
    const A& i;
    const A& j;
public:
    A(): i(*this), j(*this) {};
    const A& operator, (const A& p) const {
        return *this;
    }
    const A& operator[] (const A& a) const {
        return *this;
    }
    int operator() (const A& a, const A& b) const {
        return 0;
    }
    int f1() {
        return (i, j)[i, j](i, j);
    }
};

int main() {
    return 0;
}