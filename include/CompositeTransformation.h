#pragma once

#include <Transformation.h>
#include <EventID.h>
#include <EventType.h>

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
        ConfiguredTransformation(TransformationPtr tPtr, const EventType& out, const EventType& provided)
          : mTPtr(tPtr) {
          mOut = out;
          if(provided==EventType())
            mIn = tPtr->in(out);
          else
            mIn = tPtr->in(out, provided);
        }
        TransformationPtr trans() const { return mTPtr; }
        TransPtr create() const { return mTPtr->create(mOut, mIn); }
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
                            const EventType& provided);

    VertexResult addRootTransformation(TransformationPtr tPtr, const EventType& goal,
                                       const EventType& provided);
    VertexResult addTransformation(TransformationPtr tPtr, Vertex v,
                                   const EventType& goal, const EventType& provided);
    EventIDs inIDs() const;
    TransPtr create() const;
    bool check() const;
    const Graph& graph() const { return mGraph; }
    Vertex root() const { return mRoot; }
    TransList path(Vertex v) const;
    VertexResult find(TransformationPtr tPtr) const;
    VertexList find(const EventType& eT) const;
    bool contains(TransformationPtr tPtr) const { return find(tPtr).second; }

  friend std::ostream& operator<<(std::ostream&, const ConfiguredTransformation&);
};

std::ostream& operator<<(std::ostream& o, const CompositeTransformation& t);
