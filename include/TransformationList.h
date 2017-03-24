#pragma once

#include <Transformation.h>
#include <EventID.h>
#include <EventType.h>

#include <iosfwd>
#include <initializer_list>
#include <vector>

class TransformationList : public Transformation {
  private:
    std::vector<TransformationPtr> mTransList;
  public:
    using InitType = std::initializer_list<TransformationPtr>;

    TransformationList(InitType list);
    virtual std::vector<EventType> in(const EventType& goal) const;
    virtual std::vector<EventType> in(const EventType& goal, const  EventType& provided) const;

    virtual EventIDs in(EventID goal) const;
    virtual TransPtr create(const EventType& out, const EventTypes& in) const;
    virtual void print(std::ostream& o) const;
};
