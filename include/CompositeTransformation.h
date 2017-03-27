#pragma once

#include <ConfiguredTransformation.h>
#include <boost/graph/adjacency_list.hpp>
#include <ostream>

class CompositeTransformation : public ConfiguredTransformation {
  public:
    using  Graph = boost::adjacency_list< boost::vecS, boost::vecS, boost::bidirectionalS,
                                          ConfiguredTransformation, const EventType*>;
  private:
    Graph mGraph;
  public:

    using Vertex = Graph::vertex_descriptor;
    std::pair<Vertex, bool> addRootTransformation(TransformationPtr tPtr, const EventType& goal,
                                         EventType provided=EventType());
    std::pair<Vertex, bool> addTransformation(TransformationPtr tPtr, Vertex v,
                                         const EventType& goal, EventType provided=EventType());
    EventIDs inIDs() const;
    TransPtr create() const;
    bool check() const;
    const Graph& graph() const { return mGraph; }

  friend std::ostream& operator<<(std::ostream&, const ConfiguredTransformation&);
};

std::ostream& operator<<(std::ostream& o, const CompositeTransformation& t);
