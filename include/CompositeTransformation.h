#pragma once

#include <ConfiguredTransformation.h>
#include <Transformation.h>
#include <EventID.h>
#include <EventType.h>

#include <boost/graph/adjacency_list.hpp>

#include <ostream>
#include <utility>

class CompositeTransformation {
  public:
    using  Graph       = boost::adjacency_list< boost::vecS, boost::vecS, boost::bidirectionalS,
                                                ConfiguredTransformation, EventType>;
    using Vertex       = Graph::vertex_descriptor;
    using VertexResult = std::pair<Vertex, bool>;
    using VertexList   = std::vector<Vertex>;
    using TransPtr     = Transformation::TransPtr;
    using EventTypes   = Transformer::EventTypes;
    using EventIDs     = Transformation::EventIDs;
    using TransList    = std::vector<TransformationPtr>;

  private:
    Graph mGraph;
    EventType mOut;
    EventTypes mIn;
    Vertex mRoot;

  public:

    CompositeTransformation() = default;
    CompositeTransformation(TransformationPtr tPtr, const EventType& goal,
                            const EventType& provided);

    VertexResult addRootTransformation(TransformationPtr tPtr, const EventType& goal,
                                       const EventType& provided);
    VertexResult addTransformation(TransformationPtr tPtr, Vertex v,
                                   const EventType& goal, const EventType& provided);
    EventIDs inIDs() const;
    TransPtr create() const;
    bool check() const;
    const Graph& graph() const { return mGraph; }
    const EventType& out() const { return mOut; }
    void out(const EventType& eT) {mOut = eT; }
    const EventTypes& in() const { return mIn; }
    void in(const EventTypes& eTs) { mIn = eTs; }
    Vertex root() const { return mRoot; }
    TransList path(Vertex v) const;
    VertexResult find(TransformationPtr tPtr) const;
    VertexList find(const EventType& eT) const;
    bool contains(TransformationPtr tPtr) const { return find(tPtr).second; }

    bool operator==(const Transformer& t) const { return t==*this; }

  friend std::ostream& operator<<(std::ostream&, const ConfiguredTransformation&);
};

std::ostream& operator<<(std::ostream& o, const CompositeTransformation& t);
