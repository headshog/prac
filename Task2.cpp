#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <string.h>

using namespace std;

class BankAccount {
private:
    static int AccCnt;
    int AccNum, AccSum;
    size_t sizeowner, sizedate;
	char* AccOwner;
	char* AccDate;
    void AccAssign(const char* Owner, const char* Date, int Sum, int Num = 0, bool NewNum = false, size_t sizeow = 0, size_t sizeda = 0) {
        if (!sizeow) sizeow = strlen(Owner);
        if (!sizeda) sizeda = strlen(Date);
        AccOwner = new char[(sizeowner = sizeow) + 1];
        AccDate = new char[(sizedate = sizeda) + 1];
        strcpy(AccOwner, Owner);
        strcpy(AccDate, Date);
        if(NewNum) {
            AccCnt++;
            AccNum = AccCnt;
        }
        else
            AccNum = Num;
        AccSum = Sum;
    }
    void AccSwap(BankAccount& B) {
        swap(AccOwner, B.AccOwner);
        swap(AccDate, B.AccDate);
        swap(sizeowner, B.sizeowner);
        swap(sizedate, B.sizedate);
        swap(AccSum, B.AccSum);
        swap(AccNum, B.AccNum);
    }
public:
    BankAccount() {
        AccAssign("", "", 0);
    }
    BankAccount(const char* Owner, const char* Date, int Sum) {
        AccAssign(Owner, Date, Sum);
    }
    BankAccount(const BankAccount& B) {
        AccAssign(B.AccOwner, B.AccDate, B.AccSum, B.AccNum, false, B.sizeowner, B.sizedate);
    }
    BankAccount(BankAccount&& B) {
        AccSwap(B);
    }
    BankAccount& operator= (const BankAccount& B) {
        if (this != &B) {
            delete[] AccOwner;
            delete[] AccDate;
            AccAssign(B.AccOwner, B.AccDate, B.AccSum, 0, true, B.sizeowner, B.sizedate);
        }
        return *this;
    }
    BankAccount& operator= (BankAccount&& B) {
        if (this != &B)
            AccSwap(B);
        return *this;
    }
    friend ostream& operator<< (ostream& s, const BankAccount& B) {
		s << "----------------------\n";
        s << "Account Number: " << B.AccNum << endl;
        s << "Account Owner: " << B.AccOwner << endl;
        s << "Account Creation Date: " << B.AccDate << endl;
        s << "Account Balance: " << B.AccSum << endl;
		return s;
	}
    ~BankAccount() {
        if (AccOwner != nullptr)
            delete[] AccOwner;
        if (AccDate != nullptr)
            delete[] AccDate;
    }
};
int BankAccount::AccCnt(0);

template<typename Type>
class List {
private:
    struct Node {
        Type Elem;
        Node* Prev = nullptr, * Next = nullptr;
    };
    Node *Head, *Tail;
    size_t sz;
    void copy_list(const List& L) {
        Node *pr = nullptr, *now, *now_L = L.Head;
        for(size_t i = 0; i < L.sz; i++) {
            now = new Node;
            now->Elem = now_L->Elem;
            now->Prev = pr;
            now->Next = nullptr;
            if(i == 0)
                Head = now;
            else
                pr->Next = now;
            pr = now;
            now_L = now_L->Next;
        }
        Tail = now;
        sz = L.sz;
    }
    void swap_list(List& L) {
        Node *pr = nullptr, *now, *now_L = L.Head;
        for(size_t i = 0; i < L.sz; i++) {
            now = new Node;
            swap(now->Elem, now_L->Elem);
            now->Prev = pr;
            now->Next = nullptr;
            if(i == 0)
                Head = now;
            else
                pr->Next = now;
            pr = now;
            now_L = now_L->Next;
        }
        Tail = now;
        swap(sz, L.sz);
    }
public:
    List() : Head(nullptr), Tail(nullptr), sz(0) {}
    List(const List& L) {
        copy_list(L);
    }
    List(List&& L) {
        swap_list(L);
    }
    List& operator= (const List& L) {
        if(this != &L) {
            this->~List();
            copy_list(L);
        }
        return *this;
    }
    List& operator= (List&& L) {
        if(this != &L)
            swap_list(L);
        return *this;
    }
    ~List() {
        Node* p = Head;
        while (p != nullptr) {
            Node* q = p;
            p = p->Next;
            delete q;
        }
    }
    const Type& front() const { return Head->Elem; }
    const Type& back() const { return Tail->Elem; }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    void push_front(Type x) {
        Node* p = new Node;
        p->Elem = x;
        p->Prev = nullptr;
        p->Next = Head;
        if (Head == nullptr)
            Tail = p;
        else
            Head->Prev = p;
        Head = p;
        sz++;
    }
    void push_back(Type x) {
        Node* p = new Node;
        p->Elem = x;
        p->Prev = Tail;
        p->Next = nullptr;
        if (Tail == nullptr)
            Head = p;
        else
            Tail->Next = p;
        Tail = p;
        sz++;
    }
    void pop_front() {
        Node* p = Head;
        if (p == nullptr) {
            cout << "List is empty!\n";
            return;
        }
        Head = Head->Next;
        if (Head == nullptr)
            Tail = nullptr;
        else
            Head->Prev = nullptr;
        delete p;
        sz--;
    }
    void pop_back() {
        Node* p = Tail;
        if (p == nullptr) {
            cout << "List is empty!\n";
            return;
        }
        Tail = Tail->Prev;
        if (Tail == nullptr)
            Head = nullptr;
        else
            Tail->Next = nullptr;
        delete p;
        sz--;
    }
    void insert(size_t pos, Type x) {
        if (pos >= sz || pos < 0) {
            cout << "No such position exists!\n";
            return;
        }
        Node* pr = Head;
        for (size_t i = 0; i < pos; i++)
            pr = pr->Next;
        if (pr->Next == nullptr) {
            push_back(x);
            return;
        }
        Node* nx = pr->Next, * p = new Node;
        p->Elem = x;
        p->Prev = pr;
        p->Next = nx;
        pr->Next = p;
        nx->Prev = p;
        sz++;
    }
    void erase(size_t pos) {
        if (pos >= sz || pos < 0) {
            cout << "No such position exists!\n";
            return;
        }
        Node* p = Head;
        for (size_t i = 0; i < pos; i++)
            p = p->Next;
        if (p->Next == nullptr) {
            pop_back();
            return;
        }
        if (p->Prev == nullptr) {
            pop_front();
            return;
        }
        Node* pr = p->Prev, * nx = p->Next;
        pr->Next = nx;
        nx->Prev = pr;
        delete p;
        sz--;
    }
    void print() const {
        for (Node* p = Head; p != nullptr; p = p->Next)
            cout << p->Elem;
    }
};

template<typename Type>
class Queue : private List<Type> {
private:
    size_t max_size;
public:
    Queue(size_t size = 100) : List<Type>(), max_size(size) {};
    Queue(const Queue& q) : List<Type>(q), max_size(q.max_size) {};
    Queue(Queue&& q) : List<Type>(q), max_size(q.max_size) {};
    Queue& operator= (const Queue& q) {
        List<Type>().operator=(q);
        max_size = q.max_size;
    }
    Queue& operator= (Queue&& q) {
        List<Type>().operator=(q);
        swap(max_size, q.max_size);
    }
    const Type& front() { return List<Type>::front(); }
    size_t size() { return List<Type>::size(); }
    bool empty() { return List<Type>::empty(); }
    bool full() { return List<Type>::size() == max_size; };
    void push(Type x) { List<Type>::push_back(x); };
    void pop() { List<Type>::pop_front(); };
    void print() {List<Type>::print(); };
};

int main() {
    BankAccount Acc1("Man", "12.12.12", 123456);
    BankAccount Acc2 = Acc1;
    BankAccount Acc3 = BankAccount("Woman", "01.01.01", 123456);
    Queue<BankAccount> Q;
    Q.push(Acc1);
    Q.push(Acc2);
    Q.push(Acc3);
    Q.push(Acc2);
    Q.push(Acc2);
    Q.push(Acc1);
    cout << Q.front();
    Q.pop();
    cout << Q.front();
    Q.pop();
    Q.pop();
    Q.print();
    return 0;
}