#include <TransformationGraph.h>
#include <CompositeTransformation.h>
#include <EventTypeHelpers.h>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>

#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <ostream>

using namespace boost;
using std::vector;
using std::map;
using std::any_of;
using std::includes;
using std::sort;
using std::accumulate;
using std::find_if;
using std::ostream;
using std::pair;
using std::make_pair;


using OutIt       = TransformationGraph::OutIt;
using EventIDs    = TransformationGraph::EventIDs;
using Graph       = adjacency_list<vecS, vecS, bidirectionalS, TransformationPtr, EventID>;
using Vertex      = Graph::vertex_descriptor;
using VertexResult= pair<Vertex, bool>;
using VertexList  = vector<Vertex>;
using VIter       = Graph::vertex_iterator;
using Edge        = Graph::edge_descriptor;
using EInIter     = Graph::in_edge_iterator;
using EOutIter    = Graph::out_edge_iterator;

static VertexResult findTrans(const Graph& g, TransformationPtr tPtr) {
  auto comp = [&g, tPtr](Vertex v){ return g[v]==tPtr; };
    auto res = find_if(vertices(g).first, vertices(g).second, comp);
    if(res==vertices(g).second)
      return make_pair(Vertex(), false);
    else
      return make_pair(*res, true);
}

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
  if(result != vertices(mGraph).second) {
    clear_vertex(*result, mGraph);
  }
}

struct SpanningTreeVisitor : public default_bfs_visitor {
  const CompositeTransformation& cT;
  map<Vertex, CompositeTransformation> vTrans;
  const EventIDs& ids;
  OutIt it;
  SpanningTreeVisitor(CompositeTransformation& cT, const EventIDs& ids, OutIt it)
    : cT(cT), ids(ids), it(it) {
  }

  void discover_vertex(Vertex v, const Graph& g) {
    if(vTrans.empty())
      vTrans.insert(make_pair(v, cT));

    auto res = vTrans.find(v);
    if(res==vTrans.end())
      return;

    const CompositeTransformation& temp = res->second;
    auto check = [this](const EventType& type){ return !extractCompatibleIDs(type, ids).empty(); };
    if(all_of(temp.in().begin(), temp.in().end(), check)) {
      *it++ = temp;
    }

  }

  void tree_edge(Edge e, const Graph& g) {
    Vertex s = source(e, g);
    Vertex t = target(e, g);
    CompositeTransformation newCT = vTrans[s];
    EventTypeResult eRes = findGoalEventType(g[e], newCT.in());
    VertexResult vRes = newCT.find(g[s]);
    if(eRes.second && vRes.second) {
      newCT.addTransformation(g[t], vRes.first, eRes.first, EventType());
      vTrans[t]=move(newCT);
    }
  }
};

void TransformationGraph::generate(const EventType& type, const EventIDs& ids, OutIt it) const {

  auto gen = [this, &type, &ids](pair<OutIt, OutIt> its, Vertex v){
    if(mGraph[v]->out()==EventID(type)) {
      *its.second++ = CompositeTransformation(mGraph[v], type, EventType());
    }
    return its;
  };

  vector<CompositeTransformation> temp;
  accumulate(vertices(mGraph).first, vertices(mGraph).second, make_pair(it, back_inserter(temp)), gen);

  vector<default_color_type> colors(num_vertices(mGraph), white_color);
  boost::queue<Vertex> q;
  for(CompositeTransformation& cT : temp) {
    TransformationPtr tPtr=cT.graph()[cT.root()].trans();
    VertexResult res = findTrans(mGraph, tPtr);
    if(res.second)
    breadth_first_visit(mGraph, res.first, q, SpanningTreeVisitor(cT, ids, it),
      make_iterator_property_map(colors.begin(), get(vertex_index, mGraph))
    );
  }
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
