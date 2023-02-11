#include <iostream>
#include <string.h>

using namespace std;

class Str {
private:
    char *str;
    int size;
public:
    Str() {
        str = new char [1];
        str[0] = '\0';
        size = 0;
    }
    Str(const char* s) {
        str = new char[(size = strlen(s)) + 1];
        strcpy(str, s);
    }
    Str(const Str& s) {
        str = new char[(size = s.size) + 1];
        strcpy(str, s.str);
    }
    Str& operator= (const Str& s) {
        if(this == &s)
            return *this;
        delete[] str;
        str = new char[(size = s.size) + 1];
        strcpy(str, s.str);
        return *this;
    }
    char* get_s() {
        return str;
    }
    ~Str() {
        if(str != nullptr)
            delete[] str;
    }
};

class Man {
private:
    Str name;
    int age;
public:
    Man() : name(), age(0) {}
    Man(const char* s, int ag) : name(s), age(ag) {}
    Man(const Man& m) : name(m.name), age(m.age) {}
    Man(Man&& r) noexcept : name(), age(0) {
        name = r.name;
        age = r.age;
        r.name = Str();
        r.age = 0;
    }
    Man& operator= (Man&& r) noexcept {
        if(this == &r) 
            return *this;
        delete[] name.get_s();
        name = r.name;
        age = r.age;
        r.name = Str();
        r.age = 0;
    }
    ~Man() {}
    Man& operator= (const Man& m) {
        if(this == &m)
            return *this;
        name = m.name;
        age = m.age;
        return *this;
    }
    char* get_man_name() {
        return name.get_s(); 
    }
    Str get_name() {
        return name;
    }
    int get_man_age() {
        return age;
    }
};

int main() {
    Str s0;
    Str s1("c++");
    Str s2(s1);
    s0 = s1;
    cout << s0.get_s() << endl;
    cout << s1.get_s() << endl;
    cout << s2.get_s() << endl;
    Man m0;
    Man m1("an", 1);
    Man m2(m1);
    m0 = m2;
    cout << m0.get_man_name() << " " << m0.get_man_age() << endl;
    cout << m1.get_man_name() << " " << m1.get_man_age() << endl;
    cout << m2.get_man_name() << " " << m2.get_man_age() << endl;
    Man m3(move(m0));
    cout << m3.get_man_name() << " " << m3.get_man_age() << endl;
    return 0;
}