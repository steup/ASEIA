#include <CompositeTransformation.h>

#include <boost/graph/depth_first_search.hpp>

#include <algorithm>
#include <iterator>
#include <vector>
#include <map>


using namespace std;

using Graph      = CompositeTransformation::Graph;
using Vertex     = CompositeTransformation::Vertex;
using Edge       = Graph::edge_descriptor;
using EventTypes = CompositeTransformation::EventTypes;
using EventIDs   = CompositeTransformation::EventIDs;
using TransPtr   = CompositeTransformation::TransPtr;

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


TransPtr CompositeTransformation::create() const {
  return TransPtr();
}

bool CompositeTransformation::check() const {
  return boost::num_vertices(mGraph);
}

pair<Vertex, bool> CompositeTransformation::addRootTransformation(TransformationPtr tPtr, const EventType& tempGoal, EventType provided) {
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

pair<Vertex, bool> CompositeTransformation::addTransformation(TransformationPtr tPtr, Vertex v,
                                                              const EventType& intermediate, EventType provided) {
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

ostream& operator<<(ostream& o, const CompositeTransformation& t);
