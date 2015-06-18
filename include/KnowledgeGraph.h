#pragma once

#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>

class KnowledgeGraph{
public:

  struct NodeInfo{
	  std::string name, type;
  };

  struct EdgeInfo { };

private:
  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, NodeInfo, EdgeInfo>;

  template<class IterType, class ObjectType>
  class Iterator{
    private:
      IterType i;
      const Graph& g;
    public:
      using InternalType = IterType;
      Iterator(IterType i, const Graph& g) : i(i), g(g){}
      Iterator operator++(int){ Iterator t(*this); i++; return t;}
      Iterator& operator++(){ ++i; return *this; }
      ObjectType operator*() const { return ObjectType(*i, g); }
      bool operator!=(const Iterator& b) const { return i != b.i; }
  };

  template<class IterType>
  class IteratorAdapter{
    private:
      const IterType mBegin, mEnd;
      using GraphIter = typename IterType::InternalType;
    public:
      IteratorAdapter(GraphIter begin, GraphIter end, const Graph& g) : mBegin(begin, g), mEnd(end, g) {}
      IterType begin() const { return mBegin; }
      IterType end()   const { return mEnd; }
  };
public:
  
  class Vertex;

  class Edge {
    private:
      Graph::edge_descriptor e;
      const Graph& g;
    public:
      Edge(Graph::edge_descriptor e, const Graph& g) : e(e), g(g) {}
      operator Graph::edge_descriptor() const{ return e; }
      const EdgeInfo* operator->() const { return &g[e]; }
      Vertex source() const {return Vertex(boost::source(e, g), g); }
      Vertex target() const {return Vertex(boost::target(e, g), g); }
  };

  using VIterator  = Iterator<Graph::vertex_iterator, Vertex>;
  using Vertices   = IteratorAdapter<VIterator>;
  using EIterator  = Iterator<Graph::edge_iterator, Edge>;
  using IEIterator = Iterator<Graph::in_edge_iterator, Edge>;
  using Edges      = IteratorAdapter<EIterator>;
  using InEdges    = IteratorAdapter<IEIterator>;

  class Vertex {
    private:
      Graph::vertex_descriptor v;
      const Graph& g;
    public:
      Vertex(Graph::vertex_descriptor v, const Graph& g) : v(v), g(g) {}
      operator Graph::vertex_descriptor() const{ return v; }
      const NodeInfo* operator->() const { return &g[v]; }
      InEdges incoming() const;
    friend class KnowledgeGraph;
  };

private:
  Graph g;

public:
  KnowledgeGraph() = default;
  KnowledgeGraph(const std::string& fileName) {
    import(fileName);
  }

  void import(const std::string& fileName);

  Vertices vertices() const;
  Edges    edges() const;
  std::vector<Vertex> events() const;
  std::vector<Vertex> rules() const;

  VIterator findEvent(const std::string& name);

  bool valid(VIterator i) { return i != vertices().end(); }
};

std::ostream& operator<<(std::ostream& o, const KnowledgeGraph::Vertex &v);

std::ostream& operator<<(std::ostream& o, const KnowledgeGraph::Edge& e);

std::ostream& operator<<(std::ostream& o, KnowledgeGraph& g);
