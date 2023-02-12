#include <iostream>
#include <algorithm>
#include <cstdlib>

using namespace std;

class BankAccount {
private:
    int AccNum, AccSum;
    size_t sizeowner, sizedate;
	char* AccOwner;
	char* AccDate;
    void AccAssign(int Num, const char* Owner, const char* Date, int Sum, size_t sizeow = 0, size_t sizeda = 0) {
        if (!sizeow) sizeow = strlen(Owner);
        if (!sizeda) sizeda = strlen(Date);
        AccOwner = new char[(sizeowner = sizeow) + 1];
        AccDate = new char[(sizedate = sizeda) + 1];
        strcpy_s(AccOwner, sizeow + 1, Owner);
        strcpy_s(AccDate, sizeda + 1, Date);
        AccNum = Num;
        AccSum = Sum;
    }
public:
    BankAccount() {
        AccAssign(0, "", "", 0);
    }
    BankAccount(int Num, const char* Owner, const char* Date, int Sum) {
        AccAssign(Num, Owner, Date, Sum);
    }
    BankAccount(const BankAccount& B) {
        AccAssign(B.AccNum, B.AccOwner, B.AccDate, B.AccSum, B.sizeowner, B.sizedate);
    }
    BankAccount& operator= (const BankAccount& B) {
        if (this == &B)
            return *this;
        delete[] AccOwner;
        delete[] AccDate;
        AccAssign(B.AccNum, B.AccOwner, B.AccDate, B.AccSum, B.sizeowner, B.sizedate);
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

class List {
private:
    struct Node {
        BankAccount Elem;
        Node* Prev = nullptr, * Next = nullptr;
    };
    Node *Head, *Tail;
    size_t sz;
public:
    List() : Head(nullptr), Tail(nullptr), sz(0) {}
    ~List() {
        Node* p = Head;
        while (p != nullptr) {
            p->Elem.~BankAccount();
            p = p->Next;
        }
    }
    BankAccount front() {
        return Head->Elem;
    }
    BankAccount back() {
        return Tail->Elem;
    }
    size_t size() {
        return sz;
    }
    bool empty() {
        return sz == 0;
    }
    void push_front(BankAccount x) {
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
    void push_back(BankAccount x) {
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
    void insert(int pos, BankAccount x) {
        if (pos >= sz || pos < 0) {
            cout << "No such position exists!\n";
            return;
        }
        Node* pr = Head;
        for (int i = 0; i < pos; i++)
            pr = pr->Next;
        if (pr->Next == nullptr) {
            push_back(x);
            return;
        }
        if (pr->Prev == nullptr) {
            push_front(x);
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
    void erase(int pos) {
        if (pos >= sz || pos < 0) {
            cout << "No such position exists!\n";
            return;
        }
        Node* p = Head;
        for (int i = 0; i < pos; i++)
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
    BankAccount Acc1(1, "Man", "12.12.12", 123456);
    BankAccount Acc2 = Acc1;
    BankAccount Acc3;
    Acc3 = BankAccount(2, "Woman", "01.01.01", 123456);
    List L;
    L.push_back(Acc3);
    L.push_front(Acc1);
    L.insert(0, Acc2);
    cout << L.size() << endl;
    L.print();
    L.erase(1);
    L.pop_front();
    L.pop_back();
    cout << L.empty() << endl;
	return 0;
}