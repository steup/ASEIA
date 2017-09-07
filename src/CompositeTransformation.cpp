#include <CompositeTransformation.h>
#include <EventTypeHelpers.h>
#include <MetaEvent.h>
#include <IO.h>

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/transpose_graph.hpp>
#include <boost/graph/graphviz.hpp>

#include <algorithm>
#include <numeric>
#include <iterator>

#include <iostream>

using namespace std;

using namespace boost;

using ConfiguredTransformation = CompositeTransformation::ConfiguredTransformation;

struct CompositeTransformer : public Transformer {
    using TransPtr = Transformation::TransPtr;
    using Graph = adjacency_list<vecS, vecS, bidirectionalS, TransPtr, EventType>;
    using Vertex= Graph::vertex_descriptor;
    using Edge= Graph::edge_descriptor;

    Graph graph;
    Vertex root;
    /** \brief Generate CompositeTransformer from CompositeTransformation
     *  \param out output EventType of the created MetaEvents
     *  \param in input EventTypes of the consumed MetaEvents
     *  \param g CompositeTransformation graph to generate Transformers from
     *  \param policy policy used to generate Transformer s and Buffer s
     *  \param filter filter expressions stated by the subscriber
     **/
    CompositeTransformer(const EventType& out, const EventTypes& in,
                        const CompositeTransformation::Graph& g, const AbstractPolicy& policy, const MetaFilter& filter)
      : Transformer(out, in) {
      mFilter = filter;
      auto vertexCopy = [&g, &policy, this](CompositeTransformation::Vertex in, Vertex out) {
        graph[out] = g[in].create(policy);
      };
      Graph temp;
      copy_graph(g, graph, boost::vertex_copy(vertexCopy));
      auto it = find_if(vertices(graph).first, vertices(graph).second,
                        [this](Vertex v){ return !in_degree(v, graph);});
      root = *it;
    }

    virtual bool check(const MetaEvent& e) const {
      for(auto it = vertices(graph).first; it!=vertices(graph).second; it++)
        if(graph[*it]->check(e))
          return true;
      return false;
    }

using It = back_insert_iterator<Events>;

static It call(Vertex v, const MetaEvent& e, const Graph& graph, It it) {
    if(graph[v] == nullptr)
      return it;
    Transformer& t = *graph[v];
    Events temp = t(e);
    if(in_degree(v, graph)==0)
      return std::move(temp.begin(), temp.end(), it);
    else {
      auto edges = in_edges(v, graph);
      for(auto eIt = edges.first; eIt != edges.second; eIt++)
        for(const MetaEvent& nextE : temp)
          it=call(source(*eIt, graph), nextE, graph, it);
    }
    return it;
}

    /** \brief execute transformer on input events
     *  \param event input events
     *  \return result of the transformer graph
     **/
    virtual Events operator()(const MetaEvent& event) {
      Events result;
      EventType eT=(EventType)event;
      auto eTCompat = [eT](const EventType& eTin) {
        return eT.isCompatible(eTin);
      };
      for(auto it=vertices(graph).first; it!=vertices(graph).second;it++) {
        Transformer& t = *graph[*it];
        if(find_if(t.in().begin(), t.in().end(), eTCompat) != t.in().end())
          call(*it, event, graph, back_inserter(result));
      }
      if(!mFilter.expressions().empty())
        result.erase(remove_if(result.begin(), result.end(),
                               [this](const MetaEvent& e){ return !mFilter({&e});}),
                     result.end());
      return result;
    }

    /** \brief print composite transformer
     *  \param o output stream to print to
     **/
    void print(std::ostream& o) const {
      const Graph& g=graph;
      auto writeVertex = [&g](ostream& o, Vertex v){
        TransPtr t = g[v];
        if(t)
          o << " [label=\"" << *t << "\"]";
        else
          o << " [label=\"nullptr\"]";
        EventTypes inETs = g[v]->in();
        auto edges = out_edges(v, g);
        EventTypes outETs(edges.second-edges.first);
        EventTypes resETs(inETs.size());
        transform(edges.first, edges.second, outETs.begin(), [&g](const Edge& e){ return g[e]; });
        sort(inETs.begin(), inETs.end());
        sort(outETs.begin(), outETs.end());
        resETs.erase(set_difference(inETs.begin(), inETs.end(), outETs.begin(), outETs.end(), resETs.begin()), resETs.end());
        for(const EventType& eT: resETs) {
          o << ";\n\"" << EventID(eT) << "/" << FormatID(eT) << "\" [label=\"" << eT << "\"];\n";
          o << v << " -> \"" << EventID(eT) << "/" << FormatID(eT) << "\"";
        }
        if(in_degree(v, g)==0) {
            const EventType& eT = g[v]->out();
            o << ";\n\"" << EventID(eT) << "/" << FormatID(eT) << "\" [label=\"" << eT << "\"];\n";
            o << "\"" << EventID(eT) << "/" << FormatID(eT) << "\" -> " << v;
        }
      };
      auto writeEdge = [this](ostream& o, Edge e) {
        o << " [label=\"" << graph[e] << "\"]";
      };
      write_graphviz(o, graph, writeVertex, writeEdge);
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

CompositeTransformation::CompositeTransformation(TransformationPtr tPtr, const EventType& goal,
                                                 const EventType& provided, const MetaFilter& filter) {
  mFilter = filter;
  VertexResult res = add(tPtr, goal, provided);
  if(res.second)
    mRoot = res.first;
}

TransPtr CompositeTransformation::create(const AbstractPolicy& policy) const {
  return TransPtr(new CompositeTransformer(mOut, mIn, mGraph, policy, mFilter));
}

bool CompositeTransformation::check() const {
  return boost::num_vertices(mGraph);
}

static void extractLeafTypes(const Graph& g, back_insert_iterator<EventTypes> it) {
  auto getTypes = [&g](back_insert_iterator<EventTypes> it, Vertex v){
    const EventTypes& tempIn = g[v].in();
    EventTypes tTypes(out_degree(v, g));
    auto copyTypes = [&tTypes](const EventType& t){
      return count(tTypes.begin(), tTypes.end(), t)==0;
    };
    transform(out_edges(v, g).first,
              out_edges(v, g).second,
              tTypes.begin(),
              [&g](Edge e){ return g[e];});
    return copy_if(tempIn.begin(), tempIn.end(), it, copyTypes);
  };

  accumulate(vertices(g).first, vertices(g).second, it, getTypes);
}

VertexResult CompositeTransformation::add(TransformationPtr tPtr, const EventType& tempGoal,
                                          const EventType& provided) {
  if(num_vertices(mGraph)==0) {
    mRoot = boost::add_vertex(mGraph);
    mGraph[mRoot] = ConfiguredTransformation(tPtr, tempGoal, provided, mFilter);
    mIn=mGraph[mRoot].in();
    mOut=mGraph[mRoot].out();
    return make_pair(mRoot, true);
  }else {
    ConfiguredTransformation tempCT(tPtr, tempGoal, provided, mFilter);
    if(tempCT.in().empty() || std::find(mIn.begin(), mIn.end(), tempCT.out())!=mIn.end())
      return make_pair(Vertex(), false);
    Vertex temp = boost::add_vertex(mGraph);
    mGraph[temp] = tempCT;
    for(auto it=vertices(mGraph).first; it!=vertices(mGraph).second; it++) {
      Vertex v = *it;
      const EventTypes& in = mGraph[v].in();
      if(std::find(in.begin(), in.end(), tempCT.out())!=in.end()) {
        auto e = add_edge(v, temp, mGraph);
        if(e.second)
          mGraph[e.first] = tempCT.out();
      }
    }
    mIn.clear();
    extractLeafTypes(mGraph, back_inserter(mIn));
    std::sort(mIn.begin(), mIn.end());
    mIn.erase(std::unique(mIn.begin(), mIn.end()), mIn.end());
    return make_pair(temp, true);
  }
}

VertexResult CompositeTransformation::add(TransformationPtr tPtr, const EventType& tempGoal,
                                          const EventType& provided, const MetaFilter& filter) {
  if(num_vertices(mGraph)==0)
    mFilter = filter;
  return add(tPtr, tempGoal, provided);
}

EventTypes CompositeTransformation::allIn() const {
  EventTypes temp;
  const Graph& g = mGraph;
  auto getTypes = [&g](back_insert_iterator<EventTypes> it, Vertex v){
    const EventTypes& tempIn = g[v].in();
    auto pred = [&g, v](const EventType& eT){ return eT != g[v].out(); };
    return copy_if(tempIn.begin(), tempIn.end(), it, pred);
  };

  accumulate(vertices(g).first, vertices(g).second, back_inserter(temp), getTypes);
  std::sort(temp.begin(), temp.end());
  temp.erase(std::unique(temp.begin(), temp.end()), temp.end());
  return temp;
}

bool CompositeTransformation::addIn(const EventType& eT) {
  if(std::find(mIn.begin(), mIn.end(), eT)==mIn.end()) {
    mIn.push_back(eT);
    return true;
  } else
    return false;
}

VertexResult CompositeTransformation::add(CompositeTransformation&& cT) {
  auto checkVertex = [this, &cT](Vertex v){
    return std::find(mGraph[v].in().begin(), mGraph[v].in().end(), cT.out())==mGraph[v].in().end();
  };
  vector<Vertex> sourceVs(vertices(mGraph).first, vertices(mGraph).second);
  sourceVs.erase(remove_if(sourceVs.begin(), sourceVs.end(), checkVertex), sourceVs.end());
  if(sourceVs.empty())
    return make_pair(Vertex(), false);

  Vertex subRoot;
  EventType goal = cT.out();
  auto vertexCopy = [this, &cT, &subRoot](Vertex in, Vertex out) {
    if(in==cT.mRoot)
      subRoot = out;
    mGraph[out] = std::move(cT.mGraph[in]);
  };
  auto edgeCopy = [this, &cT](Edge in, Edge out) {
    mGraph[out] = std::move(cT.mGraph[in]);
  };
  copy_graph(cT.mGraph, mGraph, boost::vertex_copy(vertexCopy).edge_copy(edgeCopy));
  for(const Vertex& v: sourceVs) {
    auto e = boost::add_edge(v, subRoot, mGraph);
    if(!e.second)
      return make_pair(Vertex(), false);
    mGraph[e.first] = mGraph[subRoot].out();
  }
  mIn.clear();
  extractLeafTypes(mGraph, back_inserter(mIn));
  std::sort(mIn.begin(), mIn.end());
  mIn.erase(std::unique(mIn.begin(), mIn.end()), mIn.end());
  return make_pair(subRoot, true);
}

VertexResult CompositeTransformation::add(TransformationPtr tPtr, Vertex v,
                                          const EventType& intermediate,
                                          const EventType& provided) {
  const EventTypes& tempIn = mGraph[v].in();
  if(!count(tempIn.begin(), tempIn.end(), intermediate))
    return make_pair(Vertex(), false);

  Vertex newV = boost::add_vertex(mGraph);
  mGraph[newV] = ConfiguredTransformation(tPtr, intermediate, provided, mFilter);
  auto e = boost::add_edge(v, newV, mGraph);
  mGraph[e.first]=mGraph[newV].out();
  mIn.clear();
  extractLeafTypes(mGraph, back_inserter(mIn));
  std::sort(mIn.begin(), mIn.end());
  mIn.erase(std::unique(mIn.begin(), mIn.end()), mIn.end());
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
  auto pred = [&eT, this](Vertex v){
      auto innerPred = [&eT, this](Edge e){ return mGraph[e] == eT; };
      auto s = out_edges(v, mGraph).first;
      auto e = out_edges(v, mGraph).second;
      return find_if(s, e, innerPred) == e;
  };
  copy_if(vertices(mGraph).first, vertices(mGraph).second, back_inserter(result), pred);
  return result;
}

bool ConfiguredTransformation::operator==(const ConfiguredTransformation& b) const{
  if ( !(mTPtr == b.mTPtr) )
    return false;

  if ( mIn.size() != b.mIn.size())
    return false;

  if( mOut != b.mOut )
    return false;

  if(mFilter != b.mFilter)
    return false;

  if(std::equal(mIn.begin(), mIn.end(), b.mIn.begin()))
    return true;
  else
    return false;
};

bool CompositeTransformation::operator==(const CompositeTransformation& b) const {
  if(num_vertices(mGraph) != num_vertices(b.mGraph))
    return false;

  auto aVertices = vertices(mGraph);
  auto bVertices = vertices(b.mGraph);
  for(auto it = aVertices.first; it!=aVertices.second; it++) {
    auto comp = [this, &b, it](Vertex bVertex){
                  return b.mGraph[bVertex] == mGraph[*it];
                };
    if(std::find_if(bVertices.first, bVertices.second, comp)==bVertices.second)
      return false;
  }
  return true;
}

ostream& operator<<(ostream& o, const CompositeTransformation& t) {
  bool first = true;
  auto writeVertex = [&t, &first](ostream& o, Vertex v){
      const EventTypes& in = t.in();
      const Graph& g = t.graph();

      if(g[v].trans())
        o << " [label=\"" << *g[v].trans() << "\"]";
      else
        o << " [label=\"nullptr\"]";

      for(const EventType& eT: g[v].in())
        if(find(in.begin(), in.end(), eT) != in.end())
          o << ";\n" << v << " -> \"" << EventID(eT) << "/" << FormatID(eT) << "\"";

      if(in_degree(v, g)==0)
          o << ";\n\"" << EventID(t.out()) << "/" << FormatID(t.out()) << "\" -> " << v;

      if(first) {
        o << ";\n\"" << EventID(t.out()) << "/" << FormatID(t.out()) << "\" [label=\"" << t.out() << "\"]";
        for(const EventType& eT: in)
          o << ";\n\"" << EventID(eT) << "/" << FormatID(eT) << "\" [label=\"" << eT << "\"]";
        first = false;
      }
  };
  auto writeEdge = [&t](ostream& o, Edge e){
    o << " [label=\"" << t.graph()[e] << "\"]";
  };
  write_graphviz(o, t.graph(), writeVertex, writeEdge);
  return o;
}
