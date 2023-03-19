#include <iostream>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

template<typename T>
void print(T cont) {
    for(typename T::const_iterator it = cont.begin(); it != cont.end(); it++)
        cout << *it << " ";
    cout << endl;
}

template<typename T>
void print_auto(T cont) {
    for(auto it : cont)
        cout << it << " ";
    cout << endl;
}

template<typename T>
void print_reverse(T cont) {
    for(typename T::const_reverse_iterator it = cont.rbegin(); it != cont.rend(); it++)
        cout << *it << " ";
    cout << endl;
}

template<typename T>
void insert_to(vector<T>& v, const list<T>& l) {
    bool odd = true;
    typename list<T>::const_iterator itl = l.begin();
    itl++;
    for(typename vector<T>::const_iterator itv = v.begin() + 1; itv != v.end(); itv++, odd = !odd)
        if(odd) {
            itl != l.end() ? itv = v.insert(itv + 1, *itl) : itv = v.insert(itv + 1, 0);
            itl++;
            itl++;
        }
}

template<typename T>
void erase_third(T& cont) {
    int i = 1, sz = cont.size();
    for(typename T::const_iterator it = cont.begin(); i <= sz && it != cont.end(); it++, i++)
        if(i % 3 == 0)
            it = cont.erase(it), i++;
}

int main() {
    vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const list<int> lc;
    list<int> l = { 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    //const vector<int> vc1 = v;
    //const vector<int> vc2 = vector<int> (100, 123);
    //const list<int> lc1 = l;
    //print(vc1);
    //print_reverse(lc1);
    //insert_to(v, lc1);
    //print(v);
    erase_third(v);
    print_auto(v);
    erase_third(l);
    print_auto(l);
    return 0;
}