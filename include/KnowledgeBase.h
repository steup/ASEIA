#pragma once

#include <Transformation.h>
#include <CompositeTransformation.h>
#include <MetaFilter.h>

#include <vector>
#include <iosfwd>

class EventType;

class KnowledgeBase {
  public:
    using Transformations = std::vector<CompositeTransformation>;

    static void registerTransformation(const Transformation& trans);
    static void unregisterTransformation(const Transformation& trans);
    static void registerEventType(const EventType& eT);
    static void unregisterEventType(const EventType& eT);
    //TODO: query functions
    static Transformations findTransforms(const EventType& goal, const MetaFilter& filter = MetaFilter());
    static std::vector<EventType> findCompatible(const EventType& eT);
    static void clear();
    static void print(std::ostream& o);
};
