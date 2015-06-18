#include <KnowledgeGraph.h>
#include <fstream>

using namespace std;
using namespace boost;

KnowledgeGraph::Vertices KnowledgeGraph::vertices() const{
  return Vertices(boost::vertices(g).first, boost::vertices(g).second, g);
}

KnowledgeGraph::Edges KnowledgeGraph::edges() const{
  return Edges(boost::edges(g).first, boost::edges(g).second, g);
}

void KnowledgeGraph::import(const string& fileName) {
  dynamic_properties props;
  props.property("name", get(&NodeInfo::name, g));
  props.property("type", get(&NodeInfo::type, g));
  ifstream inGraph(fileName);
  read_graphml(inGraph, g, props);
}

vector<KnowledgeGraph::Vertex> KnowledgeGraph::events() const {
  vector<Vertex> storage;
  for(auto vertex : vertices())
    if( vertex->type == "event" )
      storage.push_back(vertex);
  return storage;
}

vector<KnowledgeGraph::Vertex> KnowledgeGraph::rules() const {
  vector<Vertex> storage;
  for(auto vertex : vertices())
    if( vertex->type == "rule" )
      storage.push_back(vertex);
  return storage;
}

KnowledgeGraph::VIterator KnowledgeGraph::findEvent(const string& name) {
  for(auto i = vertices().begin(); i != vertices().end(); i++)
    if( (*i)->type == "event" && (*i)->name == name)
      return i;
  return vertices().end();
}

KnowledgeGraph::InEdges KnowledgeGraph::Vertex::incoming() const {
  return InEdges(in_edges(v, g).first, in_edges(v, g).second, g);
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
