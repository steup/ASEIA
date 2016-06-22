#pragma once

#include <EventID.h>
#include <FormatID.h>

#include <utility>

class EventType;

class TypeRegistry {
	public:
		struct Key  : public std::pair<EventID, FormatID> {
			Key(const EventType& eT)
				: std::pair<EventID, FormatID>(EventID(eT), FormatID(eT))
			{}
			Key(EventID eID, FormatID fID)
				: std::pair<EventID, FormatID>(eID, fID)
			{}
		};
		static TypeRegistry& instance();
		virtual bool registerType(const EventType& eT) =0;
		virtual const EventType* operator[](const Key& key) const =0;
		virtual bool contains(const Key& k) const =0;
};
