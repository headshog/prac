#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <string.h>

using namespace std;

class BaseAbstractFigure {
public:
    enum object { Rect, Circ };
    BaseAbstractFigure(object nm) {
        name = nm;
    }
    BaseAbstractFigure(const BaseAbstractFigure& F) {
        name = F.name;
    }
    BaseAbstractFigure& operator=(const BaseAbstractFigure& F) {
        if(this != &F)
            name = F.name;
        return *this;
    }
    BaseAbstractFigure(BaseAbstractFigure&& R) {
        swap(name, R.name);
    }
    BaseAbstractFigure& operator=(BaseAbstractFigure&& R) {
        if(this != &R)
            swap(name, R.name);
        return *this;
    }
    object GetType() const {
        return name;
    }
    virtual double GetSquare() const = 0;
    virtual double GetRadius() const = 0;
    virtual double GetHeight() const = 0;
    virtual double GetWidth() const = 0;
    virtual void SetRadius(double) = 0;
    virtual void SetHeight(double) = 0;
    virtual void SetWidth(double) = 0;
    virtual ~BaseAbstractFigure() {}
protected:
    object name;
};

ostream& operator<< (ostream& s, const BaseAbstractFigure::object& t) {
    switch(t) {
        case BaseAbstractFigure::Rect: return (s << "Rectangle");
        case BaseAbstractFigure::Circ: return (s << "Circle");
    }
    return s;
}

class Rectangle : public BaseAbstractFigure {
private:
    double width, height;
    double GetRadius() const { return 0; }
    void SetRadius(double) {};
public:
    Rectangle(double w = 0, double h = 0) : BaseAbstractFigure(Rect),
                                            width(w), height(h) {};
    double GetSquare() const { return width * height; }
    double GetHeight() const { return height; }
    double GetWidth() const { return width; }
    void SetHeight(double h) { height = h; };
    void SetWidth(double w) { width = w; };
};

class Circle : public BaseAbstractFigure {
private:
    double r;
    double GetHeight() const { return 0; }
    double GetWidth() const { return 0; }
    void SetHeight(double) {};
    void SetWidth(double) {};
public:
    Circle(double rad = 0) : BaseAbstractFigure(Circ), r(rad) {};
    double GetRadius() const { return r; }
    double GetSquare() const { return 3.1415 * r * r; }
    void SetRadius(double rad) { r = rad; }
};

class ListOfForms {
private:
    BaseAbstractFigure** Form;
    size_t sz;
public:
    ListOfForms(int n, const BaseAbstractFigure& Elem) {
        Form = new BaseAbstractFigure*[n], sz = n;
        for(size_t i = 0; i < sz; i++) {
            if(Elem.GetType() == BaseAbstractFigure::Rect)
                Form[i] = new Rectangle(Elem.GetWidth(), Elem.GetHeight());
            else
                Form[i] = new Circle(Elem.GetRadius());
        }
    }
    BaseAbstractFigure& operator[](size_t i) {
        return *(Form[i]);
    }
    const BaseAbstractFigure& operator[](size_t i) const {
        return *(Form[i]);
    }
    void ReassignElem(size_t i, const BaseAbstractFigure& Elem) {
        delete Form[i];
        if(Elem.GetType() == BaseAbstractFigure::Rect)
            Form[i] = new Rectangle(Elem.GetWidth(), Elem.GetHeight());
        else
            Form[i] = new Circle(Elem.GetRadius());
    }
    size_t size() const {
        return sz;
    }
    double CountSumSquare() const {
        double sum = 0;
        for(size_t i = 0; i < sz; i++)
            sum += Form[i]->GetSquare();
        return sum;
    }
    ~ListOfForms() {
        for(size_t i = 0; i < sz; i++)
            delete Form[i];
        delete[] Form;
    }
};

int main() {
    Circle c(3);
    Rectangle r(3, 3);
    
    ListOfForms L1(5, c);
    L1[1].SetRadius(1);
    cout << L1[0].GetSquare() << endl;
    cout << L1[1].GetSquare() << endl << endl;
    
    ListOfForms L2(5, Rectangle(2, 3));
    L2[1].SetWidth(r.GetWidth());
    L2[1].SetHeight(r.GetHeight());
    cout << L2[0].GetSquare() << endl;
    cout << L2[1].GetSquare() << endl << endl;
    
    const ListOfForms L3(10, r);
    cout << L3[0].GetSquare() << endl;
    cout << L3[1].GetSquare() << endl << endl;

    cout << L3.CountSumSquare() << endl;
    return 0;
}