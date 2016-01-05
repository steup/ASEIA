#pragma once

#include <Prime.h>

#include <boost/mpl/for_each.hpp>
#include <boost/ref.hpp>

class EventID {
	public:
		using IDType = uint32_t;
	private:
		const IDType mID;

		struct IDGen {
			IDType id = 1;
			template<typename A>
			void operator()(A a){
				id*=PrimeGenerator::prime(A::IDType::value());
			}
			template<typename Event>
			IDGen(const Event& e) {
				using AttrList = typename Event::AttributeList;
				boost::mpl::for_each<AttrList>(boost::ref(*this));
			}
		};
	public:
		template<typename Event>
		EventID(const Event& e) : mID(IDGen(e).id) {

		}

		IDType value() const {
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
};
