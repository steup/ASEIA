#include <EventTypeHelpers.h>

#include <algorithm>

using namespace std;
using EventTypes = Transformation::EventTypes;
using EventIDs   = Transformation::EventIDs;

EventIDs extractCompatibleIDs(EventID goal, const EventIDs& ids) {
  EventIDs result;
  auto start = lower_bound(ids.begin(), ids.end(), goal);
  copy_if(start, ids.end(), back_inserter(result), [&goal](EventID id){ return goal<=id; });

  return result;
}

EventTypeResult findGoalEventType(EventID goal, const EventTypes& types) {
  EventTypes result;
  auto pred = [goal](const EventType& eT){ return goal <= EventID(eT); };
  auto it = find_if(types.begin(), types.end(), pred);
  if(it!=types.end())
    return make_pair(*it, true);
  else
    return make_pair(EventType(), false);
}
