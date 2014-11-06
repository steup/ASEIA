#include <boost/mpl/list.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/for_each.hpp>
#include <type_traits>
#include <utility>
#include <iostream>

template<typename A>
struct appendPair{
  template<typename List, typename B>
  struct apply{
    using pair = std::pair<A,B>;
    using extendedList = typename boost::mpl::push_front<List, pair>::type;
    using same = typename std::is_same<A,B>::type;
    using type = typename boost::mpl::if_<same, List, extendedList>::type;
  };
};

template<typename Input>
struct appendList{
  template<typename List, typename A>
    struct apply{
      using func = appendPair<A>;
      using type = typename boost::mpl::fold<Input, List, func>::type;
    };
};

template<typename... Types>
struct Variation {
  using Input = typename boost::mpl::list<Types...>::type;
  using func = appendList<Input>;
  using type = typename boost::mpl::fold<Input, boost::mpl::list<>, func>::type;
};

using Conversions = Variation<uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float, double, bool>::type;


void print(uint8_t){
  std::cout << "uint8_t";
}

void print(uint16_t){
  std::cout << "uint16_t";
}

void print(uint32_t){
  std::cout << "uint32_t";
}

void print(uint64_t){
  std::cout << "uint64_t";
}

void print(int8_t){
  std::cout << "int8_t";
}

void print(int16_t){
  std::cout << "int16_t";
}

void print(int32_t){
  std::cout << "int32_t";
}

void print(int64_t){
  std::cout << "int64_t";
}

void print(float){
  std::cout << "float";
}

void print(double){
  std::cout << "double";
}

void print(bool){
  std::cout << "bool";
}


template<typename A, typename B>
void print(std::pair<A,B>){
  std::cout << "(";
  print(A());
  std::cout << ", ";
  print(B());
  std::cout << ")";
}

struct OutputHelper{
  template<typename P>
  void operator()(P p){
    print(p);
    std::cout << std::endl;
  }
};

int main(){
  OutputHelper h;
  boost::mpl::for_each< Conversions >(h);
  return 0;
}
