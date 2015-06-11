#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

using namespace boost;
using namespace std;



class KnowledgeGraph{
public:

  struct NodeInfo{
	  string name, type;
  };

  struct EdgeInfo { };

private:
  using Graph = adjacency_list<vecS, vecS, bidirectionalS, NodeInfo, EdgeInfo>;

  template<class IterType, class ObjectType>
  class GraphIterator{
    private:
      IterType i;
      Graph& g;
    public:
      GraphIterator(IterType i, Graph& g) : i(i), g(g){}
      GraphIterator operator++(int){ GraphIterator t(*this); i++; return t;}
      GraphIterator& operator++(){ ++i; return *this; }
      ObjectType operator*() { return ObjectType(*i, g); }
      ObjectType operator*() const { return ObjectType(*i, g); }
      bool operator!=(const GraphIterator& b) { return i != b.i; }
  };
public:
  
  class InEdges;

  class Vertex {
    private:
      Graph::vertex_descriptor v;
      Graph& g;
    public:
      Vertex(Graph::vertex_descriptor v, Graph& g) : v(v), g(g) {}
      operator Graph::vertex_descriptor() const{ return v; }
      NodeInfo* operator->() const { return &g[v]; }
      InEdges incoming() const;
    friend class KnowledgeGraph;
  };

  using VIterator = GraphIterator<Graph::vertex_iterator, Vertex>;
  
  class Vertices{
    private:
      VIterator mBegin, mEnd;
    public:
      Vertices(Graph& g) : mBegin(::vertices(g).first, g), mEnd(::vertices(g).second, g){}
      VIterator begin() const { return mBegin; }
      VIterator end() const { return mEnd; }
  };

  class Edge {
    private:
      Graph::edge_descriptor e;
      Graph& g;
    public:
      Edge(Graph::edge_descriptor e, Graph& g) : e(e), g(g) {}
      operator Graph::edge_descriptor() const{ return e; }
      EdgeInfo* operator->() const { return &g[e]; }
      Vertex source() const {return Vertex(::source(e, g), g); }
      Vertex target() const {return Vertex(::target(e, g), g); }
  };

  using EIterator = GraphIterator<Graph::edge_iterator, Edge>;
  using IEIterator = GraphIterator<Graph::in_edge_iterator, Edge>;

  class Edges{
    private:
      EIterator mBegin, mEnd;
    public:
      Edges(Graph& g) : mBegin(::edges(g).first, g), mEnd(::edges(g).second, g){}
      EIterator begin() const { return mBegin; }
      EIterator end() const { return mEnd; }
  };
  
  class InEdges{
    private:
      IEIterator mBegin, mEnd;
    public:
      InEdges(Graph::vertex_descriptor v, Graph& g) : mBegin(::in_edges(v, g).first, g), mEnd(::in_edges(v, g).second, g){}
      IEIterator begin() const { return mBegin; }
      IEIterator end() const { return mEnd; }
  };

private:
  Graph g;

public:
  KnowledgeGraph() = default;
  KnowledgeGraph(const string& fileName) {
    import(fileName);
  }
  void import(const string& fileName) {
	  dynamic_properties props;
	  props.property("name", get(&NodeInfo::name, g));
	  props.property("type", get(&NodeInfo::type, g));
	  ifstream inGraph(fileName);
	  read_graphml(inGraph, g, props);
  }
  Vertices vertices() { return Vertices(g); }
  Edges edges() { return Edges(g); }
  vector<Vertex> events() {
    vector<Vertex> storage;
    for(auto vertex : vertices())
      if( vertex->type == "event" )
        storage.push_back(vertex);
    return storage;
  }
  vector<Vertex> rules() {
    vector<Vertex> storage;
    for(auto vertex : vertices())
      if( vertex->type == "rule" )
        storage.push_back(vertex);
    return storage;
  }

  VIterator findEvent(const string& name) {
    for(auto i = vertices().begin(); i != vertices().end(); i++)
      if( (*i)->type == "event" && (*i)->name == name)
        return i;
    return vertices().end();
  }

  bool valid(VIterator i) { return i != vertices().end(); }
};

KnowledgeGraph::InEdges KnowledgeGraph::Vertex::incoming() const {
  return KnowledgeGraph::InEdges(v, g);
}

std::ostream& operator<<(std::ostream& o, const KnowledgeGraph::Vertex &v){
	return o << "(" << v->name << ": " << v->type << ")";
}

std::ostream& operator<<(std::ostream& o, const KnowledgeGraph::Edge& e){
	return o << e.source() << " -> " << e.target();
}

std::ostream& operator<<(std::ostream& o, KnowledgeGraph& g){
	o << "vertices:" <<  endl;
	for(const auto& vertex : g.vertices())
		o << "\t" << vertex << endl;
	o << "edges:"  << endl;
	for(const auto& edge : g.edges())
	o << "\t" << edge << endl;
	return o;
}

class  IncompleteCmdError : public std::exception {
	const char* msg;
	public:
		IncompleteCmdError(const string& desc) : msg(desc.c_str()){}
		virtual const char* what() const throw() { return msg; }
};

namespace po = boost::program_options;

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

int main(int argc, char** argv){
	KnowledgeGraph g;
	try {
		g.import(parseOptions(argc, argv));
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

	cout << g << endl;

  cout << "Events:" << endl;
  for(const auto& event : g.events()) {
    cout << "\t" << event << endl;
  }

  cout << "Rules:" << endl;
  for(const auto& rule : g.rules())
    cout << "\t" << rule << endl;
 
  auto posIter = g.findEvent("Position");

  if(g.valid(posIter)) {
    cout << "Interesting rules for Position:" << endl;
    for(const auto& rule : (*posIter).incoming())
      cout << "\t" << rule.source() << endl;
	  return 0;
  }
}
