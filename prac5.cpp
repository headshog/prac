#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <optional>

using namespace std;

class A {
public:
    static int t;
    int n;
    A(int a = 0) : n(a) {};
    /*friend ostream& operator<< (ostream& s, const A& t) {
        s << t.n;
        return s;
    }*/
    A operator+ (const A& b) const {
        return n + b.n;
    }
};
int A::t(5);

template<class T>
T add(const T t1, const T t2) {
    return t1 + 1 + t2;
}

int main() {
    A a(5), b(5);
    cout << add(3, A::t) << endl;
    cout << add(a, b).n << endl;
    return 0;
}