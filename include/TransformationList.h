#pragma once

#include <Transformation.h>
#include <EventID.h>
#include <EventType.h>

#include <iosfwd>
#include <initializer_list>
#include <vector>

class TransformationList : public Transformation, public std::vector<TransformationPtr> {
  public:
    using InitType = std::initializer_list<TransformationPtr>;

    TransformationList(Type type, EventID id) : Transformation(type, 1, id) {}
    virtual std::vector<EventType> in(const EventType& goal) const;
    virtual std::vector<EventType> in(const EventType& goal, const  EventType& provided) const;

    virtual EventIDs in(EventID goal) const;
    virtual TransPtr create(const EventType& out, const EventTypes& in) const;
    virtual void print(std::ostream& o) const;
};
