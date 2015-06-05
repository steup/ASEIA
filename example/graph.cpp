#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

struct VertexProperty{
	std::string name;
};

struct EdgeProperty{
	std::string transformation;
};

using Graph = adjacency_list<vecS, vecS, directedS, VertexProperty, EdgeProperty>;

// Make convenient labels for the vertices
enum Vertices { A, B, C, D, E};
const char names[] = "ABCDE";

// writing out the edges in the graph
using Edge   = std::tuple<std::string, Vertices, Vertices>;
using Vertex = graph_traits<Graph>::vertex_descriptor;
using VIter  = graph_traits<Graph>::vertex_iterator;
using EIter  = graph_traits<Graph>::edge_iterator;
using OEIter  = graph_traits<Graph>::out_edge_iterator;
using VPair  = std::pair<VIter, VIter>; 
using EPair  = std::pair<EIter, EIter>; 
using OEPair  = std::pair<OEIter, OEIter>; 

template <class Graph> struct handleVertex {
	using Vertex = typename graph_traits<Graph>::vertex_descriptor;
	const Graph& g;

	handleVertex(const Graph& g) : g(g) {}

	void operator()(const Vertex& v) const {
//		std::cout << "out_edges of " << v << " = [";
//		for(const auto& edge : out_edges(v, g))
//			std::cout << edge << 
	}
};

template <class Graph> struct handleEdge {
	using Edge = typename graph_traits<Graph>::edge_descriptor;
	const Graph& g;

	handleEdge(const Graph& g) : g(g) {}

	void operator()(const Edge& e) const {
	}
};

namespace std{
	VIter begin(VPair pair) { return pair.first; }
	VIter end(VPair pair) { return pair.second; }
	EIter begin(EPair pair) { return pair.first; }
	EIter end(EPair pair) { return pair.second; }
}

std::ostream& operator<<(std::ostream& o, std::pair<Graph::vertex_descriptor, const Graph&> in){
	return o << in.second[in.first].name;
}

std::ostream& operator<<(std::ostream& o, std::pair<Graph::edge_descriptor, const Graph&> in){
	return o << in.second[in.first].transformation << ": " << std::make_pair(source(in.first, in.second), in.second) << " -> " << std::make_pair(target(in.first, in.second), in.second);
}

std::ostream& operator<<(std::ostream& o, const Graph& g){
	o << "vertices = [";
	for(const auto& vertex : vertices(g))
		o << std::make_pair(vertex, g) << " ";
	o << "]" << std::endl;
	o << "edges    = ";
	for(const auto& edge : edges(g))
		o << "(" << std::make_pair(edge, g) << ") ";
	return o;
}

int main()
{
	Graph g;

	for(const auto& vertex : {Vertices::A, Vertices::B, Vertices::C, Vertices::D, Vertices::E})
		g[add_vertex(g)].name=names[vertex];

	for(const auto& edge : { 
		Edge("1", Vertices::A,Vertices::B), 
		Edge("2", Vertices::A,Vertices::D), 
		Edge("3", Vertices::C,Vertices::A), 
		Edge("4", Vertices::D,Vertices::C),
		Edge("5", Vertices::C,Vertices::E), 
		Edge("6", Vertices::B,Vertices::D), 
		Edge("7", Vertices::D,Vertices::E)
	}){
		auto res = add_edge(std::get<1>(edge), std::get<2>(edge), g);
		if(res.second)
			g[res.first].transformation=std::get<0>(edge);
	}
	std::cout << g << std::endl;

	auto vHandle = handleVertex<Graph>(g);
	for(const auto& vertex : vertices(g))
		vHandle(vertex);

	auto eHandle = handleEdge<Graph>(g);
  for(const auto& edge : edges(g))
  	eHandle(edge);

	return 0;
}
