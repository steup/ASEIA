#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>

using namespace boost;
using namespace std;

using Graph = adjacency_list<vecS, vecS, directedS>;

const char* graphMLFile = "/home/steup/test.graphml";

int main(){
	ifstream inGraph(graphMLFile);
	Graph g;
	dynamic_properties props;
	try {
		read_graphml(inGraph, g, props);
		cout << "Parsing of GraphML file: " << graphMLFile << " successfull" << endl;
	}
	catch(const bad_parallel_edge& e){
		cerr << "Unsupported parallel edge on parsing GraphML file: " << graphMLFile << endl;
		cerr << e.what() << endl;
	}
	catch(const directed_graph_error& e){
		cerr << "Directed Graph Error on parsing GraphML file: " << graphMLFile << endl;
		cerr << e.what() << endl;
	}
	catch(const undirected_graph_error& e){
		cerr << "Undirected Graph Error on parsing GraphML file: " << graphMLFile << endl;
		cerr << e.what() << endl;
	}
	catch(const parse_error& e){
		cerr << "Parse Error on parsing GraphML file: " << graphMLFile << endl;
		cerr << e.what() << endl;
	}
	catch(const graph_exception& e){
		cerr << "Graph Exception on parsing GraphML file: " << graphMLFile << endl;
		cerr << e.what() << endl;
	}
	catch(const std::exception& e){
		cerr << "Exception on parsing GraphML file: " << graphMLFile << endl;
		cerr << e.what() << endl;
	}
}
