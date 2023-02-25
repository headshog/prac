#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>

using namespace std;

class Arr {
   int * arr;
   int size;
public:  
   Arr(int s, int date = 0) : size(s) { 
      arr = new int[s]; 
      for(int i = 0; i < size; arr[i++] = date);
   }
   Arr(const Arr& a) {
      arr = new int[size = a.size];
      memcpy(arr, a.arr, size * sizeof(int));
   }
   Arr(Arr&& a) {
      cout << "copy rval\n";
      swap(arr, a.arr);
      swap(size, a.size);
   }
   ~Arr () { delete [] arr;}
   friend ostream& operator<< (ostream& s, const Arr& a) {
		for(int i = 0; i < a.size; i++)
         s << a.arr[i] << " ";
      s << endl;
		return s;
	}
   Arr& operator= (const Arr& a) {
      if(this == &a)
         return *this;
      delete[] arr;
      arr = new int[size = a.size];
      memcpy(arr, a.arr, size * sizeof(int));
      return *this;
   }
   Arr& operator= (Arr&& a) {
      cout << "operator= rval\n";
      if(this == &a)
         return *this;
      swap(arr, a.arr);
      swap(size, a.size);
      return *this;
   }
   Arr operator| (int a) {
      Arr tmp(*this);
      for(int i = 0; i < tmp.size; i++)
         tmp.arr[i] /= a;
      return tmp;
   }
   Arr& operator|= (int a) {
      for(int i = 0; i < size; i++)
         arr[i] /= a;
      return *this;
   }
   Arr operator-() const {
      Arr tmp(*this);
      for(int i = 0; i < size; i++)
         tmp.arr[i] = -tmp.arr[i];
      return tmp;
   }
   Arr operator| (const Arr& a) const {
      Arr tmp(*this);
      for(int i = 0; i < min(tmp.size, a.size); i++)
         if(a.arr[i] != 0)
            tmp.arr[i] /= a.arr[i];
      return tmp;
   }
   int& operator[] (int i) {
      return this->arr[i];
   }
};

int  main()  {      
   Arr a1(15), a2(20, 5), a3(30, 3);    
   const Arr a4(5);
   cout << a1 << a2 << a3 << a4;
   a2 = a2 | 2;  // объект a2 здесь не меняется
   a2 |= 1;  // а здесь меняется
   a3 = (-a2) | (-a4);
   a3[5] = 10;
   a1 = a4 | a3;
   cout << a1 << a2 << a3 << a4;
   return 0;
}