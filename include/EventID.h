#pragma once

#include <cstdint>

class EventType;

class EventID {
	public:
		using IDType = std::uint32_t;
	private:
		IDType mID;
    static IDType idGen(const EventType& eT);
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

		/** \brief strict superset test **/
		bool operator>(const EventID& b) const {
			return !((*this)<=b) && b.value();
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
