#include <CompositeTransformation.h>
#include <EventTypeHelpers.h>

#include <boost/graph/depth_first_search.hpp>

#include <algorithm>
#include <iterator>
#include <vector>
#include <map>


using namespace std;

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

struct FindTransformation : public boost::default_dfs_visitor {
  VertexResult& result;
  TransformationPtr tPtr;

  FindTransformation(TransformationPtr tPtr, VertexResult& result)
    : result(result), tPtr(tPtr)
  {}

  void discover_vertex(Vertex v, const Graph& g) {
    if(g[v].trans()==tPtr) {
      result.first = v;
      result.second =true;
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
  return TransPtr();
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
  VertexResult result(Vertex(), false);
  boost::depth_first_search(mGraph, boost::visitor(FindTransformation(tPtr, result)));
  return result;
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
