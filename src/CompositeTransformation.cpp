#include <CompositeTransformation.h>

#include <boost/graph/depth_first_search.hpp>

#include <algorithm>
#include <iterator>
#include <vector>
#include <map>


using namespace boost;
using std::copy;
using std::vector;
using std::back_inserter;
using std::map;

struct InputEventTypeExtractor : public default_dfs_visitor {
  using Graph      = CompositeTransformation::Graph;
  using Vertex     = CompositeTransformation::Vertex;
  using Edge       = CompositeTransformation::Edge;
  using EventTypes = Transformation::EventTypes;
  using OutEdgeIter= Graph::out_edge_iterator;
  Vertex temp;
  EventTypes& in;
  EventType goal, provided;
  map<Edge, EventType> types;

  InputEventTypeExtractor(EventTypes& in, const EventType& goal, EventType provided=EventType())
    : in(in), goal(goal), provided(provided)
  {}

  void discover_vertex(Vertex v, const Graph& g) {
    temp = v;
    auto inIDs = in_edges(v, g);
    EventType tempGoal = (inIDs.first != inIDs.second)?types[*inIDs.first]:goal;
    EventTypes tempIn = (provided!=EventType())?g[v]->in(tempGoal, provided):g[v]->in(tempGoal);
    auto outEdges = out_edges(v, g);
    for(OutEdgeIter i = outEdges.first; i != outEdges.second; i++) {
      EventID edgeID = g[*i];
      auto it = find_if(tempIn.begin(), tempIn.end(), [edgeID](const EventType& e){ return EventID(e)>=edgeID; });
      types[*i]=*it;
    }
  }

  void finish_vertex(Vertex v, const Graph& g) {
    if(temp == v) {
      auto inIDs = in_edges(v, g);
      EventType tempGoal = (inIDs.first != inIDs.second)?types[*inIDs.first]:goal;
      EventTypes tempIn = (provided!=EventType())?g[v]->in(tempGoal, provided):g[v]->in(tempGoal);
      copy(tempIn.begin(), tempIn.end(), back_inserter(in));
    }
  }
};

struct InputEventIDExtractor : public default_dfs_visitor {
  using Graph    = CompositeTransformation::Graph;
  using Vertex   = CompositeTransformation::Vertex;
  using Edge     = CompositeTransformation::Edge;
  using EventIDs = Transformation::EventIDs;
  Vertex temp;
  EventIDs& in;
  EventID goal;

  InputEventIDExtractor(EventIDs& in, EventID goal)
    : in(in), goal(goal)
  {}

  void discover_vertex(Vertex v, const Graph& g) {
    temp = v;
  }

  void finish_vertex(Vertex v, const Graph& g) {
    if(temp == v) {
      auto inIDs = in_edges(v, g);
      if(inIDs.first != inIDs.second) {
        Edge e = *inIDs.first;
        EventID tempGoal = g[e];
        EventIDs tempIn = g[v]->in(tempGoal);
        copy(tempIn.begin(), tempIn.end(), back_inserter(in));
      }
    }
  }
};

CompositeTransformation::CompositeTransformation()
  : Transformation(Type::composite, 0, EventID::any) {

}

Transformation::EventTypes CompositeTransformation::in(const EventType& goal) const {
  EventTypes result;
  InputEventTypeExtractor extractTypes(result, goal);
  depth_first_search(graph, visitor(extractTypes));
  return result;
}

Transformation::EventTypes CompositeTransformation::in(const EventType& goal, const  EventType& provided) const {
  EventTypes result;
  InputEventTypeExtractor extractTypes(result, goal, provided);
  depth_first_search(graph, visitor(extractTypes));
  return result;
}

Transformation::EventIDs CompositeTransformation::in(EventID goal) const {
  EventIDs result;
  InputEventIDExtractor  extractIDs(result, goal);
  depth_first_search(graph, visitor(extractIDs));
  return result;
}

Transformation::TransPtr CompositeTransformation::create(const EventType& out, const EventTypes& in) const {
  return TransPtr();
}

void CompositeTransformation::print(std::ostream& o) const {

}
