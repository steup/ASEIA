#pragma once

#include <Transformation.h>
#include <EventID.h>

#include <utility>

using EventTypeResult = std::pair<EventType, bool>;

EventTypeResult findGoalEventType(EventID goal, const Transformation::EventTypes& types);

/** \brief extract comaptible EventIds  from published EventIds
 *  \param goal the goal EventID
 *  \param ids all published EventIds
 **/
Transformation::EventIDs extractCompatibleIDs(EventID goal, const Transformation::EventIDs& ids);
