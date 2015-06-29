#pragma once

#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>

class NodeInfo{
public:
	virtual bool isEventType() const = 0;
	virtual bool isTransformation() const = 0;
	virtual bool operator==(const NodeInfo& b) const = 0;
};

class EventType2 : public NodeInfo{
public:
	enum Types{
		Position,
		Speed,
		Accel,
		Proximity,
		Void
	};
private:
	const Types mType;
	public:
	EventType2() : NodeInfo(), mType(Void) {}
	EventType2(Types t) : NodeInfo(), mType(t) {}
	virtual bool isEventType() const { return true; }
	virtual bool isTransformation() const { return false; }
	Types type() const { return mType; }
	virtual bool operator==(const NodeInfo& n) const { return n.isEventType() && mType == dynamic_cast<const EventType2&>(n).mType; }
};

class Transformation : public NodeInfo{
	public:
		Transformation() = default;
		virtual bool isEventType() const { return false; }
		virtual bool isTransformation() const { return true; }
		virtual unsigned int arity() const =0;
	  virtual bool operator==(const NodeInfo& n) const { return n.isTransformation(); }
};

class EdgeInfo{
	private:
		const unsigned int mCardinality;
	public:
		EdgeInfo() : mCardinality(0) {}
		EdgeInfo(unsigned int cardinality) : mCardinality(cardinality) {}
		unsigned int cardinality() const { return mCardinality; }
};

std::ostream& operator<<(std::ostream& o, const EdgeInfo& e){
	return o << e.cardinality();
}

std::ostream& operator<<(std::ostream& o, const EventType2& e){
	switch(e.type()){
		case(EventType2::Position):  return o  << "Position";
		case(EventType2::Speed):     return o  << "Speed";
		case(EventType2::Accel):     return o  << "Accel";
		case(EventType2::Proximity): return o  << "Proximity";
		case(EventType2::Void):      return o  << "Void";
		default:                    return o << "Unknown Event Type";
	}
}
std::ostream& operator<<(std::ostream& o, const NodeInfo& n){
	if(n.isEventType())
		return o << dynamic_cast<const EventType2&>(n);
	if(n.isTransformation())
		return o << dynamic_cast<const Transformation&>(n);
	return o << "Unknown Node Type";
}

std::ostream& operator<<(std::ostream& o, const Transformation& t){
	return o << t.arity() << "-ary Transformation";
}

class KnowledgeGraph{
private:
	
  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, std::unique_ptr<NodeInfo>, std::unique_ptr<EdgeInfo>>;

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
      const EdgeInfo* operator->() const { return g[e].get(); }
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
			const NodeInfo& operator*() const { return *g[v];}
      const NodeInfo* operator->() const { return g[v].get(); }
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

  VIterator findEvent(const EventType2& e);

  bool valid(VIterator i) { return i != vertices().end(); }

	void insertEventType(const EventType2& e);
	void insertTransformation(const Transformation& t);
};

std::ostream& operator<<(std::ostream& o, const KnowledgeGraph::Vertex &v);

std::ostream& operator<<(std::ostream& o, const KnowledgeGraph::Edge& e);

std::ostream& operator<<(std::ostream& o, KnowledgeGraph& g);
