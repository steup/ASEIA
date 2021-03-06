#pragma once

#include <Transformation.h>
#include <EventID.h>
#include <EventType.h>
#include <MetaFilter.h>

#include <boost/graph/adjacency_list.hpp>

#include <ostream>
#include <utility>

class CompositeTransformation : public AbstractConfiguredTransformation {
  public:
    class ConfiguredTransformation : public AbstractConfiguredTransformation {
      private:
        TransformationPtr mTPtr;
      public:
        using TransPtr = Transformation::TransPtr;
        ConfiguredTransformation() = default;
        ConfiguredTransformation(TransformationPtr tPtr, const EventType& out,
                                 const EventType& provided = EventType(),
                                 const MetaFilter& filter = MetaFilter())
          : mTPtr(tPtr) {
          mFilter = filter;
          mOut = out;
          mIn = tPtr->in(out, provided, mFilter);
        }
        TransformationPtr trans() const { return mTPtr; }
        TransPtr create(const AbstractPolicy& policy) const {
          return mTPtr->create(mOut, mIn, policy, mFilter);
        }
        bool operator==(const ConfiguredTransformation& b) const;
    };
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
    Vertex mRoot;

  public:

    CompositeTransformation() = default;
    CompositeTransformation(TransformationPtr tPtr, const EventType& goal,
                            const EventType& provided, const MetaFilter& filter = MetaFilter());

    VertexResult add(CompositeTransformation&& cT);
    VertexResult add(TransformationPtr tPtr, const EventType& goal,
                     const EventType& provided, const MetaFilter& filter);
    VertexResult add(TransformationPtr tPtr, const EventType& goal,
                     const EventType& provided = EventType());
    VertexResult add(TransformationPtr tPtr, Vertex v, const EventType& goal,
                     const EventType& provided = EventType());
    EventIDs inIDs() const;
    EventTypes allIn() const;
    bool addIn(const EventType&);
    TransPtr create(const AbstractPolicy& policy) const;
    bool check() const;
    const Graph& graph() const { return mGraph; }
    Vertex root() const { return mRoot; }
    TransList path(Vertex v) const;
    VertexResult find(TransformationPtr tPtr) const;
    VertexList find(const EventType& eT) const;
    bool contains(TransformationPtr tPtr) const { return find(tPtr).second; }
    bool operator==(const CompositeTransformation& b) const;
};

std::ostream& operator<<(std::ostream& o, const CompositeTransformation& t);
