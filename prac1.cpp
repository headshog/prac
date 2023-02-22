#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

class Drob {
private:
    int m, n;
    static int cnt;
public:
    Drob(int a = 0, int b = 1): m(a), n(b) {cnt++;}
    int gcd (int a, int b) {
	    return b ? gcd (b, a % b) : a;
    }
    bool operator== (const Drob& b) {
        int gcd_a = gcd(m, n), gcd_b = gcd(b.m, b.n);
        return (m / gcd_a == b.m / gcd_b) && (n / gcd_a == b.n / gcd_b);
    }
    const Drob operator+ (const Drob& b) {
        return {m * b.n + b.m * n, n * b.n};
    }
    const Drob operator- (const Drob& b) {
        return {m * b.n - b.m * n, n * b.n};
    }
    const Drob operator* (const Drob& b) {
        return {m * b.m, n * b.n};
    }
    void print() {
        cout << m << '/' << n << endl;
    }
};
int Drob::cnt(0);

class BaseAnimal {
private:
    int hunger, max_hunger, size, food_type;
    char* pet_name;
public:
    BaseAnimal(int h = 0, int h_max = 0, int food = 0, const char* name = nullptr): hunger(h), max_hunger(h_max), food_type(food) {
        if(name != nullptr) {
            pet_name = new char[(size = strlen(name) + 1)];
            strcpy(pet_name, name);
        }
        else {
            pet_name = new char[1];
            pet_name[0] = '\n';
        }
    }
    ~BaseAnimal() {
        delete[] pet_name;
    }
    void tick() {
        if(hunger < max_hunger)
            hunger++;
    }
    bool is_hungry() {
        if(hunger == max_hunger)
            cout << pet_name << ": feed me!\n";
        return hunger == max_hunger;
    }
    void feed(int food) {
        if(food == food_type) {
            hunger = 0;
            cout << pet_name << " feeded!\n";
        }
        else
            cout << pet_name << ": wrong food!\n";
    }
};

class Wolf: public BaseAnimal {
public:
    Wolf(int h_max = 0, int h = 0): BaseAnimal(h, h_max, 1, "Wolf") {}
};

class Lion: public BaseAnimal {
public:
    Lion(int h_max = 0, int h = 0): BaseAnimal(h, h_max, 1, "Lion") {}
};

class Tiger: public BaseAnimal {
public:
    Tiger(int h_max = 0, int h = 0): BaseAnimal(h, h_max, 1, "Tiger") {}
};

class Giraffe: public BaseAnimal {
public:
    Giraffe(int h_max = 0, int h = 0): BaseAnimal(h, h_max, 2, "Giraffe") {}
};

class Deer: public BaseAnimal {
public:
    Deer(int h_max = 0, int h = 0): BaseAnimal(h, h_max, 2, "Deer") {}
};

int main() {
    /*
    Drob a(2, 3), b(1, 2);
    Drob c = (a + b);
    c.print();
    */
    Wolf w(100, 1);
    Tiger t(200, 1);
    Lion l(300, 1);

    Giraffe g(400, 2);
    Deer d(500, 2);

    for(int i = 0; i < 500; i++) {
        //w.tick();
        //t.tick();
        l.tick();
        //g.tick();
        //d.tick();
        if(w.is_hungry())
            w.feed(1);
        if(t.is_hungry())
            t.feed(1);
        if(l.is_hungry())
            l.feed(2);
        if(g.is_hungry())
            g.feed(1);
        if(d.is_hungry())
            d.feed(1);
    }

   return 0;
}