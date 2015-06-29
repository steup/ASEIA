#pragma once

#include <ostream>
#include <cstdint>
#include <initializer_list>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/interval.hpp>

using namespace boost::numeric::interval_lib;
//using namespace compare::set;

class F : public boost::numeric::interval< float, 
                                           policies< save_state< rounded_transc_std< float > >,
                                                     checking_base< float > 
                                           > > {
	public:
	using BaseType =  boost::numeric::interval<float, policies<save_state<rounded_transc_std<float>>, checking_base<float>>>; 
	F() : BaseType(0,0){}
  F(float a) : BaseType(a,a) {}
  F(const BaseType& i) : BaseType(i) {}
	F(std::initializer_list<float> l){
		float tempA = 0;
		float tempB = 0;
		auto lIter = l.begin();
		if(lIter != l.end()){
			tempA = *lIter++;
      if(lIter != l.end()) {
			  tempB = *lIter++;
        assign(tempA-tempB, tempA+tempB);
       } else
        assign(tempA, tempA);
     } else {
        assign(0,0);
     }
	}
};

static auto Pi = pi<F::BaseType>();

F atan2(const F& y, const F& x){
  if(x>0.0f)
    return boost::numeric::atan(y/x);
  if(y>=0.0f && x<0.0f)
    return boost::numeric::atan(y/x)+Pi;
  if(y<0.0f && x<0.0f)
    return boost::numeric::atan(y/x)-Pi;
  if(y>0.0f && x==0)
    return Pi/2.0f;
  if(y<0.0f && x==0)
    return -Pi/2.0f;
  return 0.0f;
}

std::ostream& operator<<(std::ostream& o, const F& f){
	return o << "[" << (f.lower()+f.upper())/2 << "\u00B1" << (f.upper()-f.lower())/2 << "]";
}

using F2 = boost::numeric::ublas::vector<F, boost::numeric::ublas::bounded_array<F, 2>>;


enum class EventType {Position, Speed, Accel, Proximity, Void};

class Pose{
	public:
	F2 pos;
	F  theta;
  Pose() : pos(2) {};
	Pose(std::initializer_list<F> l) : pos(2) {
		auto lIter = l.begin();
		if(lIter != l.end())
			pos(0) = *lIter++;
		if(lIter != l.end())
			pos(1) = *lIter++;
		if(lIter != l.end())
			theta = *lIter*Pi/180.0f;
	}
};

std::ostream& operator<<(std::ostream& o, const Pose& p){
	return o << "(" << p.pos(0) << "," << p.pos(1) << ":" << p.theta*180.0f/Pi << ")";
}

class Polar{
	public:
	F d;
	F theta;
  Polar() = default;
	Polar(F d, F theta) :  d(d), theta(theta) {}
	Polar(std::initializer_list<F> l){
		auto lIter = l.begin();
		if(lIter != l.end())
			d=*lIter++;
		if(lIter != l.end())
			theta = *lIter*Pi/180.0f;
	}
};

std::ostream& operator<<(std::ostream& o, const Polar& p){
	return o << "(" << p.d << ":" << p.theta*180.0f/Pi << ")";
}

class Event{
	public:
	const EventType type;
	uint64_t objectID;
	Pose pose;
	F time;
	Event(EventType type, uint64_t id, Pose pose, F time)
		: type(type),
		  objectID(id),
		  pose(pose),
		  time(time)
	{}
	Event(EventType type) : type(type){}
	Event()	: type(EventType::Void)	{}

	virtual std::ostream& print(std::ostream& o) const {
		return o << "[" << objectID << "]: " << time << " " << pose;
	}
};

class SpeedEvent : public Event{
	public:
	Polar speed;
	SpeedEvent(uint64_t id, Pose pose, F time, Polar speed)
		: Event(EventType::Speed, id, pose, time),
		  speed(speed)
	{}
	SpeedEvent() : Event(EventType::Speed) {}
	std::ostream& print(std::ostream& o) const{
		return Event::print(o << "Speed: ") << " " << speed;
	}
};

class PositionEvent : public Event{
	public:
	PositionEvent(uint64_t id, Pose pose, F time) :
		Event(EventType::Position, id, pose, time)
	{}
	PositionEvent() : Event(EventType::Position) {}
	std::ostream& print(std::ostream& o) const{
		return Event::print(o << "Position");
	}
};

class ProximityEvent : public Event{
	public:
	Polar distance;
	uint64_t secondObject;

	ProximityEvent(uint64_t id, Pose pose, F time, Polar distance) :
		Event(EventType::Proximity, id, pose, time), distance(distance)
	{}
	ProximityEvent() : Event(EventType::Proximity) {}
	std::ostream& print(std::ostream& o) const{
		return Event::print(o << "Distance")<<" "<<distance << " to " <<  secondObject;
	}
};

class AccelEvent : public Event{
	public:
	Polar accel;

	AccelEvent(uint64_t id, Pose pose, F time, Polar accel) :
		Event(EventType::Accel, id, pose, time), accel(accel)
	{}
	AccelEvent() : Event(EventType::Accel) {}
	std::ostream& print(std::ostream& o) const{
		return Event::print(o << "Accel")<<" "<<accel;
	}
};

std::ostream& operator<<(std::ostream& o, const Event& e){
	return e.print(o);
}

std::ostream& operator<<(std::ostream& o, EventType t){
	switch(t){
		case(EventType::Position):  return o << "Position";
		case(EventType::Speed):     return o << "Speed";
		case(EventType::Accel):     return o << "Accel";
		case(EventType::Proximity): return o << "Proximity";
		case(EventType::Void):      return o << "Void";
		default:                    return o << "Unknown";
	}
}
