#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>

using namespace std;

template<typename T>
void print(T& cont) {
    for(auto it : cont)
        cout << it << " ";
    cout << endl;
}

// Клаcc-функция увеличения всех элементов
// контейнера на какое-то заданное число.
// Работает и с массивами встр. типов.
// Помимо operator() я сделал еще конструктор
// с теми же параметрами для того, чтобы можно
// было вызывать то действие вот так:
// IncShift(arr, val);
class IncShift {
public:
    IncShift() {}

    template<typename T, typename Tval>
    IncShift(T& arr, Tval shift_val = 0) {
        for(auto& it : arr)
            it += shift_val;
    }

    template<typename T, typename Tval>
    void operator() (T& arr, Tval shift_val = 0) {
        for(auto& it : arr)
            it += shift_val;
    }
};

int main() {
    IncShift make_inc_shift;

    vector<int> v = { 1, 2, 3, 4, 5 };
    list<int> l = { 1, 2, 3, 4, 5 };
    string s = "abcde";
    int i[5] = { 1, 2, 3, 4, 5 };
    double d[5] = { 1.1, 2.1, 3.1, 4.1, 5.1 };

    cout << "vector:\n";
    make_inc_shift(v, 1);
    print(v);
    cout << endl;

    cout << "list:\n";
    make_inc_shift(l, 2);
    print(l);
    cout << endl;

    cout << "string:\n";
    make_inc_shift(s, 3);
    print(s);
    cout << endl;

    cout << "int array:\n";
    make_inc_shift(i, 1);
    print(i);
    cout << endl;    
    
    cout << "double array:\n";
    make_inc_shift(d, 0.9);
    print(d);
    cout << endl;

    cout << "double array modified by constructor call:\n";
    IncShift(d, 0.9);
    print(d);
    cout << endl;

    return 0;
}