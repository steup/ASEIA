#include <iostream>

// Testcase of runtime datatype arithmetic using polymorphic behaviour of C++
//
// Approach is viable, but memory consumption is too high: 
//
// AVR: 2 byte ptr +  4 byte v_table + 1,2,4,8 byte data  = 7,8,10,14
// x86: 4 byte ptr +  8 byte v_table + 1,2,4,8 byte data  = 13,14,16,20
// x64: 8 byte ptr + 16 byte v_table + 1,2,4,8 byte data  = 25,26,28,32
//
// Union based system: 8 byte for all

using namespace std;

struct Element{
  enum class Type{
    Integer,
    Float
  } type;
  Element(Type type) : type(type){}
  virtual Element& operator+=(const Element&) = 0;
  virtual ~Element(){}
};

struct Integer : public Element{
  int i;
  Integer(int i) : Element(Element::Type::Integer), i(i){}
  virtual Element& operator+=(const Element& e){
    i+=dynamic_cast<const Integer&>(e).i;
    return *this;
  }
};

struct Float : public Element{
  float f;
  Float(float f) : Element(Element::Type::Float), f(f){}
  virtual Element& operator+=(const Element& e){
    f+=dynamic_cast<const Float&>(e).f;
    return *this;
  }
};

struct ElementProxy{
  Element* ptr;
  ElementProxy(int value){
    ptr = new Integer(value);
  }
  ElementProxy(float value){
    ptr = new Float(value);
  }
  ElementProxy(const ElementProxy& b) : ptr(nullptr){
    switch(b.ptr->type){
      case(Element::Type::Integer): ptr = new Integer(*dynamic_cast<const Integer*>(b.ptr));
                                    break;
      case(Element::Type::Float): ptr = new Float(*dynamic_cast<const Float*>(b.ptr));
                                  break;
    }
  }
  ~ElementProxy(){
    delete ptr;
  }

  ElementProxy& operator+=(const ElementProxy& b){
    *ptr+=*b.ptr;
    return *this;
  }

  ElementProxy operator+(const ElementProxy& b){
    ElementProxy temp=*this;
    temp+=b;
    return temp;
  }

  void cast(Element::Type type){
    Element* temp = nullptr;
    switch(type){
      case(Element::Type::Integer): ptr = new Integer(
  }
};

ostream& operator<<(ostream& o, const ElementProxy& i){
  switch(i.ptr->type){
    case(Element::Type::Integer): return o << dynamic_cast<const Integer*>(i.ptr)->i;
    case(Element::Type::Float): return o << dynamic_cast<const Float*>(i.ptr)->f;
  }
  return o;
}

int main(){
  ElementProxy a(10);
  ElementProxy b(20);
  cout << "a   : " << a << endl;
  cout << "b   : " << b << endl;
  cout << "a+b : " << (a+b) << endl;
  ElementProxy c(1.1f);
  ElementProxy d(2.2f);
  cout << "c   : " << c << endl;
  cout << "d   : " << d << endl;
  cout << "c+d : " << (c+d) << endl;
  return 0;
}
