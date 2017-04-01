#include <TransformationGraph.h>
#include <CompositeTransformation.h>

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>

#include <vector>
#include <algorithm>
#include <numeric>
#include <ostream>

using namespace boost;
using std::vector;
using std::any_of;
using std::includes;
using std::sort;
using std::accumulate;
using std::find_if;
using std::ostream;

using OutIt      = TransformationGraph::OutIt;
using EventIDs   = TransformationGraph::EventIDs;
using Graph      = adjacency_list<vecS, vecS, bidirectionalS, TransformationPtr, EventID>;
using Vertex     = Graph::vertex_descriptor;
using VertexList = vector<Vertex>;
using VIter      = Graph::vertex_iterator;
using Edge       = Graph::edge_descriptor;
using EInIter    = Graph::in_edge_iterator;
using EOutIter   = Graph::out_edge_iterator;

/** \todo implement transformation links **/
void TransformationGraph::insert(TransformationPtr tPtr) {
  auto pred = [this, tPtr](Vertex v){ return mGraph[v]==tPtr; };
  if(any_of(vertices(mGraph).first, vertices(mGraph).second, pred))
    return;
  Vertex newV = add_vertex(mGraph);
  mGraph[newV]=tPtr;
  EventIDs inNew = tPtr->in(tPtr->out());
  for(VIter it = vertices(mGraph).first; it != vertices(mGraph).second; it++) {
    if(*it==newV)
      continue;
    TransformationPtr bPtr = mGraph[*it];
    EventIDs in = bPtr->in(bPtr->out());
    if(count(inNew.begin(), inNew.end(), bPtr->out())) {
      auto e = add_edge(newV, *it, mGraph);
      mGraph[e.first]=bPtr->out();
    }
    if(count(in.begin(), in.end(), tPtr->out())) {
      auto e = add_edge(*it, newV, mGraph);
      mGraph[e.first]=tPtr->out();
    }
  }
}

void TransformationGraph::remove(TransformationPtr tPtr) {
  auto pred = [this, tPtr](Vertex v){ return mGraph[v]==tPtr; };
  VIter result = find_if(vertices(mGraph).first, vertices(mGraph).second, pred);
  if(result != vertices(mGraph).second)
    clear_vertex(*result, mGraph);
}

/** \todo implement spanning tree based search **/
void TransformationGraph::generate(const EventType& type, const EventIDs& ids, OutIt it) const {
  auto gen = [this, &type, &ids](OutIt it, Vertex v){
    if(mGraph[v]->out()==EventID(type)) {
      EventIDs in = mGraph[v]->in(EventID(type));
      sort(in.begin(), in.end());
      if(includes(ids.begin(), ids.end(), in.begin(), in.end()))
        *it++ = CompositeTransformation(mGraph[v], type, EventType());
    }
    return it;
  };
  accumulate(vertices(mGraph).first, vertices(mGraph).second, it, gen);
  //Insert Spanning Tree code
}

void TransformationGraph::clear() {
  mGraph = Graph();
}

ostream& operator<<(ostream& o, const TransformationGraph& g) {
  auto vertexWriter = [&g](ostream& o, Vertex v){
    o << " [label=\"" << *g.mGraph[v] << "\"]";
  };
  auto edgeWriter = [&g](ostream& o, Edge e){
    o << " [label=\"" << g.mGraph[e] << "\"]";
  };
  write_graphviz(o, g.mGraph, vertexWriter, edgeWriter);
  return o;
}
