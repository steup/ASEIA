#pragma once

#include <Prime.h>
#include <EventType.h>

class EventID {
	public:
		using IDType = uint32_t;
	private:
		const IDType mID;
    static uint32_t idGen(const EventType& eT) {
      uint32_t id=1;
      for(const AttributeType& aT : eT)
        id *= PrimeGenerator::prime(aT.id());
      return id;
    }
		constexpr EventID() : mID(0) {}
	public:

		template<typename Event>
    EventID(const Event& e)
      : mID(idGen(EventType(e)))
    {}

		EventID(const EventType& eT)
      : mID(idGen(eT))
    {}

		IDType value() const {
			return mID;
		}

    operator uint32_t() const {
      return mID;
    }

		bool operator==(const EventID& b) const {
			return value() == b.value();
		}

		/** \brief subset test **/
		bool operator<=(const EventID& b) const {
			return b.value() % value() == 0;
		}

		bool operator!=(const EventID& b) const {
			return !((*this)==b);
		}

		/** \brief strict superset test **/
		bool operator>(const EventID& b) const {
			return !((*this)<=b);
		}

		/** \brief strict subset test **/
		bool operator<(const EventID& b) const {
			return (*this)<=b && (*this)!=b;
		}

		bool operator>=(const EventID& b) const {
			return (*this)>b || (*this)==b;
		}
		
		static const EventID any;
};
