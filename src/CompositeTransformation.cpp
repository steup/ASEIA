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

    struct CallVisitor : public default_dfs_visitor {

      Events& results;
      map<Vertex, vector<MetaEvent>> buffer;

      const MetaEvent& input;

      CallVisitor(Events& results, const MetaEvent& input) : results(results), input(input) {}

      void initialize_vertex(Vertex v, const Graph& g) {
        buffer.emplace(v, vector<MetaEvent>());
      }

      void finish_vertex(Vertex v, const Graph& g) {
        vector<MetaEvent> temp = g[v]->operator()(input);
        std::move(temp.begin(), temp.end(), back_inserter(buffer[v]));
        auto edges = out_edges(v, g);
        for(auto it=edges.first; it!=edges.second; it++)
          for(const MetaEvent& e: buffer[target(*it, g)]) {
            temp = g[v]->operator()(e);
            std::move(temp.begin(), temp.end(), back_inserter(buffer[v]));
          }
        results = buffer[v];
      }
    };

    Graph graph;
    Vertex root;
    /** \brief Generate CompositeTransformer from CompositeTransformation
     *  \param out output EventType of the created MetaEvents
     *  \param in input EventTypes of the consumed MetaEvents
     *  \param g CompositeTransformation graph to generate Transformers from
     **/
    CompositeTransformer(const EventType& out, const EventTypes& in,
                        const CompositeTransformation::Graph& g, const AbstractPolicy& policy)
      : Transformer(out, in) {
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

    /** \brief execute transformer on input events
     *  \param events input events
     *  \return result of the transformer graph
     **/
    virtual Events operator()(const MetaEvent& event) {
      Events result;
      vector<default_color_type> colors(num_vertices(graph));
      depth_first_visit(graph, root, CallVisitor(result, event),
                        make_iterator_property_map(colors.begin(), get(vertex_index, graph)));
      return result;
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

TransPtr CompositeTransformation::create(const AbstractPolicy& policy) const {
  if(boost::num_vertices(mGraph)==1) {
    return mGraph[mRoot].create(policy);
  }
  return TransPtr(new CompositeTransformer(mOut, mIn, mGraph, policy));
}

bool CompositeTransformation::check() const {
  return boost::num_vertices(mGraph);
}

VertexResult CompositeTransformation::addRootTransformation(TransformationPtr tPtr, const EventType& tempGoal,
                                                            const EventType& provided) {
  if(num_vertices(mGraph)==0) {
    Vertex root = boost::add_vertex(mGraph);
    mGraph[root] = ConfiguredTransformation(tPtr, tempGoal, provided);
    mIn=mGraph[root].in();
    mOut=mGraph[root].out();
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
  mGraph[newV] = ConfiguredTransformation(tPtr, intermediate, provided);
  auto e = boost::add_edge(v, newV, mGraph);
  mGraph[e.first]=mGraph[newV].out();
  mIn.clear();
  boost::depth_first_search(mGraph, boost::visitor(InputEventTypeExtractor(mIn)));
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

ostream& operator<<(ostream& o, const CompositeTransformation& t) {
  const Graph& g=t.graph();
  auto writeVertex = [&g](ostream& o, Vertex v){
      if(g[v].trans())
        o << " [label=\"" << *g[v].trans() << "\"]";
      else
        o << " [label=\"nullptr\"]";
  };
  auto writeEdge = [&g](ostream& o, Edge e){
    o << " [label=\"" << g[e] << "\"]";
  };
  write_graphviz(o, g, writeVertex, writeEdge);
  return o;
}
