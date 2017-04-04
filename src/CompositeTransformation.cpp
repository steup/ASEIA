#include <CompositeTransformation.h>
#include <EventTypeHelpers.h>
#include <MetaEvent.h>

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/transpose_graph.hpp>
#include <boost/graph/graphviz.hpp>

#include <algorithm>
#include <iterator>


using namespace std;

using namespace boost;


struct CompositeTransformer : public Transformer {
    using TransPtr = Transformation::TransPtr;
    using Graph = adjacency_list<vecS, vecS, bidirectionalS, TransPtr>;
    using Vertex= Graph::vertex_descriptor;
    using Edge= Graph::edge_descriptor;

    Graph graph;
    /** \brief Generate CompositeTransformer from CompositeTransformation
     *  \param out output EventType of the created MetaEvents
     *  \param in input EventTypes of the consumed MetaEvents
     *  \param g CompositeTransformation graph to generate Transformers from
     **/
    CompositeTransformer(const EventType& out, const EventTypes& in, const CompositeTransformation::Graph& g)
      : Transformer(Transformation::invalid, out, in) {
      auto vertexCopy = [&g, this](CompositeTransformation::Vertex in, Vertex out) {
        graph[out] = g[in].create();
      };
      auto edgeCopy = [&g, this](CompositeTransformation::Graph::edge_descriptor in, Edge out) {};
      Graph temp;
      transpose_graph(g, graph, boost::vertex_copy(vertexCopy).edge_copy(edgeCopy));
    }

    /** \brief check for applicability  of Transformer
     *  \param  events input events to check
     *  \return true if transformer can be applied, false otherwise
     *  \todo implement **/
    bool check(const Events& events) const {
      return false;
    }

    /** \brief execute transformer on input events
     *  \param events input events
     *  \return result of the transformer graph
     *  \todo implement
     **/
    MetaEvent operator()(const Events& events) {
      return MetaEvent();
    }

    /** \brief print composite transformer
     *  \param o output stream to print to
     **/
    void print(std::ostream& o) const {
      auto writeVertex = [this](ostream& o, Vertex v){
        TransPtr t = graph[v];
        if(t)
          o << " [label=\"" << *t << "\"]";
        else
          o << " [label=\"nullptr\"]";
      };
      write_graphviz(o, graph, writeVertex);
    }
};

using Graph        = CompositeTransformation::Graph;
using Vertex       = CompositeTransformation::Vertex;
using VertexResult = CompositeTransformation::VertexResult;
using VertexList   = CompositeTransformation::VertexList;
using Edge         = Graph::edge_descriptor;
using EventTypes   = CompositeTransformation::EventTypes;
using EventIDs     = CompositeTransformation::EventIDs;
using TransPtr     = CompositeTransformation::TransPtr;
using TransList    = CompositeTransformation::TransList;

struct InputEventTypeExtractor : public boost::default_dfs_visitor {
  Vertex temp;
  EventTypes& in;

  InputEventTypeExtractor(EventTypes& in)
    : in(in)
  {}

  void discover_vertex(Vertex v, const Graph& g) {
    temp = v;
  }

  void finish_vertex(Vertex v, const Graph& g) {
    if(temp == v) {
      const EventTypes& tempIn = g[v].in();
      copy(tempIn.begin(), tempIn.end(), back_inserter(in));
    }
  }
};

struct FindEventType : public boost::default_dfs_visitor {
  VertexList& result;
  const EventType& eT;

  FindEventType(const EventType& eT, VertexList& result)
    : result(result), eT(eT)
  {}

  void discover_vertex(Vertex v, const Graph& g) {
    if(count(g[v].in().begin(), g[v].in().end(), eT))
      result.push_back(v);
  }
};
;

CompositeTransformation::CompositeTransformation(TransformationPtr tPtr, const EventType& goal,
                                                 const EventType& provided) {
  VertexResult res = addRootTransformation(tPtr, goal, provided);
  if(res.second)
    mRoot = res.first;
}

TransPtr CompositeTransformation::create() const {
  if(boost::num_vertices(mGraph)==1) {
    return mGraph[mRoot].create();
  }
  return TransPtr(new CompositeTransformer(mOut, mIn, mGraph));
}

bool CompositeTransformation::check() const {
  return boost::num_vertices(mGraph);
}

VertexResult CompositeTransformation::addRootTransformation(TransformationPtr tPtr, const EventType& tempGoal,
                                                            const EventType& provided) {
  if(num_vertices(mGraph)==0) {
    Vertex root = boost::add_vertex(mGraph);
    ConfiguredTransformation& cT = mGraph[root];
    cT.trans(tPtr);
    cT.out(tempGoal);
    if(provided==EventType())
      cT.in(tPtr->in(tempGoal));
    else
      cT.in(tPtr->in(tempGoal, provided));
    in(cT.in());
    out(cT.out());
    return make_pair(root, true);
  }else
    return make_pair(Vertex(), false);
}

VertexResult CompositeTransformation::addTransformation(TransformationPtr tPtr, Vertex v,
                                                        const EventType& intermediate,
                                                        const EventType& provided) {
  const EventTypes& tempIn = mGraph[v].in();
  if(!count(tempIn.begin(), tempIn.end(), intermediate))
    return make_pair(Vertex(), false);

  Vertex newV = boost::add_vertex(mGraph);
  ConfiguredTransformation& cT = mGraph[newV];
  cT.trans(tPtr);
  cT.out(intermediate);
  if(provided==EventType())
    cT.in(tPtr->in(intermediate));
  else
    cT.in(tPtr->in(intermediate, provided));
  auto e = boost::add_edge(v, newV, mGraph);
  mGraph[e.first]=&cT.out();
  mIn.clear();
  boost::depth_first_search(mGraph, boost::visitor(InputEventTypeExtractor(mIn)));
  //sort(mIn.begin(), mIn.end());
  //mIn.erase(unique(mIn.begin(). mIn.end()), mIn.end());
  return make_pair(newV, true);
}

EventIDs CompositeTransformation::inIDs() const {
  EventIDs ids(mIn.size());
  copy(mIn.begin(), mIn.end(), ids.begin());
  return ids;
}

TransList CompositeTransformation::path(Vertex v) const {
  TransList result;
  bool done=false;
  do {
    result.push_back(mGraph[v].trans());
    auto in=in_edges(v, mGraph);
    if(in.first!=in.second)
      v=source(*in.first, mGraph);
    else
      done=true;
  }while(!done);
  return result;
}

VertexResult CompositeTransformation::find(TransformationPtr tPtr) const {
  auto  pred = [this, tPtr](Vertex v){ return mGraph[v].trans()==tPtr; };
  auto it = find_if(vertices(mGraph).first, vertices(mGraph).second, pred);
  if(it==vertices(mGraph).second)
    return make_pair(Vertex(), false);
  else
    return make_pair(*it, true);
}

VertexList CompositeTransformation::find(const EventType& eT) const {
  VertexList result;
  boost::depth_first_search(mGraph, boost::visitor(FindEventType(eT, result)));
  return result;
}

/** \todo implement **/
ostream& operator<<(ostream& o, const CompositeTransformation& t) {
  return o;
}
