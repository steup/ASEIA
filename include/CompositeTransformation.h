#pragma once

#include <Transformation.h>
#include <boost/graph/adjacency_list.hpp>

class CompositeTransformation : public Transformation {
  public:
   using  Graph = boost::adjacency_list< boost::vecS, boost::vecS, boost::bidirectionalS,
                                         TransformationPtr, EventID>;
   using Vertex = Graph::vertex_descriptor;
   using Edge   = Graph::edge_descriptor;
  Graph graph;

  CompositeTransformation();
  virtual EventTypes in(const EventType& goal) const;
  virtual EventTypes in(const EventType& goal, const  EventType& provided) const;

  virtual EventIDs in(EventID goal) const;
  virtual TransPtr create(const EventType& out, const EventTypes& in) const;
  virtual void print(std::ostream& o) const;
};
