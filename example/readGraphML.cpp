#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>

using namespace boost;
using namespace std;

struct NodeInfo{
	string name, type;
};


using Graph = adjacency_list<vecS, vecS, directedS, NodeInfo>;

const char* graphMLFile = "/home/steup/test.graphml";

using VIter  = graph_traits<Graph>::vertex_iterator;
using EIter  = graph_traits<Graph>::edge_iterator;
using VPair  = std::pair<VIter, VIter>; 
using EPair  = std::pair<EIter, EIter>; 
namespace std{
	VIter begin(VPair pair) { return pair.first; }
	VIter end(VPair pair) { return pair.second; }
	EIter begin(EPair pair) { return pair.first; }
	EIter end(EPair pair) { return pair.second; }
}

std::ostream& operator<<(std::ostream& o, std::pair<Graph::vertex_descriptor, const Graph&> in){
	return o << "(" << in.second[in.first].name << ": " << in.second[in.first].type << ")";
}

std::ostream& operator<<(std::ostream& o, std::pair<Graph::edge_descriptor, const Graph&> in){
	return o << "(" << std::make_pair(source(in.first, in.second), in.second) << " -> " << std::make_pair(target(in.first, in.second), in.second) <<  ")";
}

std::ostream& operator<<(std::ostream& o, const Graph& g){
	o << "vertices = [";
	for(const auto& vertex : vertices(g))
		o << std::make_pair(vertex, g) << " ";
	o << "]" << std::endl;
	o << "edges    = [";
	for(const auto& edge : edges(g))
		o << std::make_pair(edge, g) << " ";
	return o  << "]";
}

int main(){
	ifstream inGraph(graphMLFile);
	Graph g;
	dynamic_properties props;
	props.property("name", get(&NodeInfo::name, g));
	props.property("type", get(&NodeInfo::type, g));
	try {
		read_graphml(inGraph, g, props);
		cout << "Parsing of GraphML file: " << graphMLFile << " successfull" << endl;
		cout << g << endl;
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
		cerr << e.error << ": " << e.statement << endl;
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
