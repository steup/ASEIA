#include <TransformationGraph.h>
#include <CompositeTransformation.h>

#include <boost/graph/depth_first_search.hpp>

#include <vector>
#include <algorithm>
#include <numeric>

using namespace boost;
using std::vector;
using std::any_of;
using std::includes;
using std::sort;
using std::accumulate;
using std::find_if;

using OutIt      = TransformationGraph::OutIt;
using EventIDs   = TransformationGraph::EventIDs;
using Graph      = adjacency_list<vecS, vecS, bidirectionalS, TransformationPtr, EventID>;
using Vertex     = Graph::vertex_descriptor;
using VertexList = vector<Vertex>;
using VIter      = Graph::vertex_iterator;
using Edge       = Graph::edge_descriptor;
using EInIter    = Graph::in_edge_iterator;
using EOutIter   = Graph::out_edge_iterator;

void TransformationGraph::insert(TransformationPtr tPtr) {
  auto pred = [this, tPtr](Vertex v){
    return mGraph[v]==tPtr;
  };
  if(any_of(vertices(mGraph).first, vertices(mGraph).second, pred))
    return;
  Vertex v = add_vertex(mGraph);
  mGraph[v]=tPtr;
}

void TransformationGraph::remove(TransformationPtr tPtr) {
  auto pred = [this, tPtr](Vertex v){
    return mGraph[v]==tPtr;
  };
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
  for(VIter it = vertices(mGraph).first; it!=vertices(mGraph).second; it++)
    clear_vertex(*it, mGraph);
}
