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
    void AccAssign(const char* Owner, const char* Date, int Sum, bool Num = false, size_t sizeow = 0, size_t sizeda = 0) {
        if (!sizeow) sizeow = strlen(Owner);
        if (!sizeda) sizeda = strlen(Date);
        AccOwner = new char[(sizeowner = sizeow) + 1];
        AccDate = new char[(sizedate = sizeda) + 1];
        strcpy(AccOwner, Owner);
        strcpy(AccDate, Date);
        if(Num) {
            AccCnt++;
            AccNum = AccCnt;
        }
        else
            AccNum = 0;
        AccSum = Sum;
    }
public:
    BankAccount() {
        AccAssign("", "", 0);
    }
    BankAccount(const char* Owner, const char* Date, int Sum) {
        AccAssign(Owner, Date, Sum);
    }
    BankAccount(const BankAccount& B) {
        AccAssign(B.AccOwner, B.AccDate, B.AccSum, false, B.sizeowner, B.sizedate);
    }
    BankAccount& operator= (const BankAccount& B) {
        if (this != &B) {
            delete[] AccOwner;
            delete[] AccDate;
            AccAssign(B.AccOwner, B.AccDate, B.AccSum, true, B.sizeowner, B.sizedate);
        }
        return *this;
    }
    ~BankAccount() {
        if (AccOwner != nullptr)
            delete[] AccOwner;
        if (AccDate != nullptr)
            delete[] AccDate;
    }
    void print_bank_acc() {
        cout << "----------------------\n";
        cout << "Account Number: " << AccNum << endl;
        cout << "Account Owner: " << AccOwner << endl;
        cout << "Account Creation Date: " << AccDate << endl;
        cout << "Account Balance: " << AccSum << endl;
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
public:
    List() : Head(nullptr), Tail(nullptr), sz(0) {}
    List(const List& L) {
        copy_list(L);
    }
    List& operator= (const List& L) {
        if(this != &L) {
            this->~List();
            copy_list(L);
        }
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
    Type front() {
        return Head->Elem;
    }
    Type back() {
        return Tail->Elem;
    }
    size_t size() {
        return sz;
    }
    bool empty() {
        return sz == 0;
    }
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
    void print() {
        for (Node* p = Head; p != nullptr; p = p->Next)
            p->Elem.print_bank_acc();
    }
};

int main() {
    BankAccount Acc1("Man", "12.12.12", 123456);
    BankAccount Acc2 = Acc1;
    BankAccount Acc3 = BankAccount("Woman", "01.01.01", 123456);
    List<BankAccount> L;
    L.push_back(Acc3);
    L.push_back(Acc3);
    L.push_front(Acc1);
    L.insert(0, Acc2);
    L.insert(1, Acc2);
    L.insert(2, Acc2);
    L.insert(L.size() - 1, Acc1);    
    cout << L.size() << endl;
    L.print();
    L.erase(1);
    L.erase(0);
    L.erase(L.size() - 1);
    L.pop_front();
    L.pop_back();
    L.print();
    cout << L.empty() << endl;
    return 0;
}