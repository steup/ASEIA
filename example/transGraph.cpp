#include <iostream>
#include <iomanip>
//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>

#include "transEvent.h"
#include "transFunc.h"

int main(){
  std::cout << std::setprecision(1) << std::fixed;
	PositionEvent p0(0, {{0, 0.1},{1, 0.1},{0, 0.2}}, {10, 1});
	PositionEvent p1(0, {{5, 0.1},{3, 0.1},{0, 0.2}}, {100, 1});
	Event& e0 = p0;
	Event& e1 = p1;
	PosToSpeed pts; 
	Trans2& t= pts;
	std::cout << e0 << std::endl;
	std::cout << e1 << std::endl;
	std::cout << t.transform(e0, e1) << std::endl;
	return 0;
}
