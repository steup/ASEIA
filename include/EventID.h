#pragma once

#include <ID.h>

#include <cstdint>
#include <initializer_list>

class EventType;

class EventID {
	public:
		using IDType = std::uint32_t;
    using InitType = std::initializer_list<uint8_t>;
	private:
		IDType mID;
    static IDType idGen(const EventType& eT);
    static IDType idGen(const InitType& attrs);
	public:
		constexpr EventID() : mID(0) {}

		template<typename Event>
    EventID(const Event& e)
      : mID(idGen(EventType(e)))
    {}

		EventID(const EventType& eT)
      : mID(idGen(eT))
    {}

    EventID(const InitType& attrs)
      : mID(idGen(attrs))
    {}

		IDType value() const {
			return mID;
		}

    operator IDType() const {
      return mID;
    }

		bool operator==(const EventID& b) const {
			return value() == b.value();
		}

		/** \brief subset test **/
		bool operator<=(const EventID& b) const {
			return value() && (b.value() % value() == 0);
		}

		bool operator!=(const EventID& b) const {
			return !((*this)==b);
		}

		/** \brief strict superset test
     **/
		bool operator>(const EventID& b) const {
			return b<(*this);
		}

		/** \brief strict subset test **/
		bool operator<(const EventID& b) const {
			return (*this)<=b && (*this)!=b;
		}

		bool operator>=(const EventID& b) const {
			return b<=(*this);
		}

    EventID& operator/=(id::attribute::ID attr);
    EventID& operator*=(id::attribute::ID attr);
    EventID operator/(id::attribute::ID attr) const { return EventID(*this)/=attr; }
    EventID operator*(id::attribute::ID attr) const { return EventID(*this)*=attr; }

    static bool comp(EventID a, EventID b) { return a.mID < b.mID; }

		static const EventID any;
};
