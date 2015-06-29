#include <iostream>                  // for std::cout
#include <KnowledgeGraph.h>


int main(){
	KnowledgeGraph g;
	g.insertEventType(EventType2(EventType2::Position));
	for( const auto& v : g.events())
		std::cout << v << std::endl;
	return 0;
}
