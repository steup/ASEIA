#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

using namespace boost;
namespace po = boost::program_options;
using namespace std;

struct NodeInfo{
	string name, type;
};

using Graph = adjacency_list<vecS, vecS, directedS, NodeInfo>;

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
	return o << std::make_pair(source(in.first, in.second), in.second) << " -> " << std::make_pair(target(in.first, in.second), in.second);
}

std::ostream& operator<<(std::ostream& o, const Graph& g){
	o << "vertices:" <<  endl;
	for(const auto& vertex : vertices(g))
		o << "\t" << std::make_pair(vertex, g) << endl;
	o << "edges:"  << endl;
	for(const auto& edge : edges(g))
		o << "\t" << std::make_pair(edge, g) << endl;
	return o;
}

class  IncompleteCmdError : public std::exception {
	const char* msg;
	public:
		IncompleteCmdError(const string& desc) : msg(desc.c_str()){}
		virtual const char* what() const throw() { return msg; }
};

string parseOptions(int argc, char** argv){
	po::options_description desc("commandline options");
	desc.add_options()
    ("help", "produce help message")
    ("akg", po::value<string>(), "aseia knowledge graph file");

	po::variables_map vm;
	po::store(parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    

	if( !vm.count("akg") || vm.count("help")) {
		ostringstream o;
		o << desc;
    throw IncompleteCmdError(o.str());
	} else
		return vm["akg"].as<string>();
}

void readAKG(Graph& g, const string& fileName){
	dynamic_properties props;
	props.property("name", get(&NodeInfo::name, g));
	props.property("type", get(&NodeInfo::type, g));
	ifstream inGraph(fileName);
	read_graphml(inGraph, g, props);
}

int main(int argc, char** argv){
	Graph g;
	try {
		readAKG(g, parseOptions(argc, argv));
		cout << g << endl;
	}
	catch(const IncompleteCmdError& e){
		cerr << argv[0] << " " << e.what() << endl;
		return -1;
	}
	catch(const graph_exception& e){
		cerr << argv[0] << ": Error on parsing graphml: " << endl << e.what() << endl;
		return -1;
	}
	catch(const std::exception& e){
		cerr << argv[0] << ": Unknown exception occured" << endl << e.what() << endl;
		return -1;
	}
	return 0;
}
