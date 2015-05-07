#include <iostream>
//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>

#include <cstdint>
#include <tgmath.h>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/interval.hpp>

using namespace std;
using namespace boost::numeric::interval_lib::compare::set;

class F : public boost::numeric::interval<float>{
	public:
	using BaseType =  boost::numeric::interval<float>;
	F() : BaseType(0,0){}
	F(float a) : BaseType(a,a){}
	F(float a, float alpha) : BaseType(a-alpha,a+alpha){}
	F(initializer_list<float> l){
		float tempA = 0;
		float tempB = 0;
		auto lIter = l.begin();
		if(lIter != l.end())
			tempA = *lIter++;
		if(lIter != l.end())
			tempB = *lIter++;
		F(tempA, tempB);
	}
};

ostream& operator<<(ostream& o, const F& f){
	return o << "[" << (f.lower()+f.upper())/2 << "\u00B1" << (f.upper()-f.lower())/2;
}

using VF = boost::numeric::ublas::vector<F>;


enum class EventType {Position, Speed, Accel, Proximity, Void};

class Pose{
	public:
	VF pos;
	F  theta;
	Pose(F x, F y, F theta) :  theta(theta) {
		pos(0)=x;
		pos(1)=y;
	}
	Pose(initializer_list<F> l){
		auto vIter = pos.begin();
		auto lIter = l.begin();
		while(vIter != pos.end() && lIter != l.end())
			*vIter++=*lIter++;
		if(lIter != l.end())
			theta = *lIter;
	}
};

class Polar{
	public:
	F d;
	F theta;
	Polar(F d, F theta) :  d(d), theta(theta) {}
	Polar(initializer_list<F> l){
		auto lIter = l.begin();
		if(lIter != l.end())
			d=*lIter++;
		if(lIter != l.end())
			theta = *lIter;
	}
};

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
	EventType(EventType type) : type(type){}
	Event()	: type(EventType::Void)	{}

	virtual ostream& print(ostream& o) const{
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
	ostream& print(ostream& o) const{
		return Event::print(o << "Speed: ") << " " << speed;
	}
};

class PositionEvent : public Event{
	public:
	PositionEvent(uint64_t id, Pose pose, F time) :
		Event(EventType::Position, id, pose, time)
	{}
	PositionEvent() : Event(EventType::Position) {}
	ostream& print(ostream& o) const{
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
	ostream& print(ostream& o) const{
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
	ostream& print(ostream& o) const{
		return Event::print(o << "Accel")<<" "<<accel;
	}
};

class Trans{
	protected:
		const EventType mOutType;
		const unsigned int mNumIn;
	public:
		Trans(EventType outType, unsigned int numIn)
			: mOutType(outType),
			  mNumIn(numIn)
		{}
};

class Trans1 : public Trans{
	protected:
		const EventType mInType;
	public:
		Trans2(EventType out, EventType in)
			: Trans(out, 1),
			  mInType(in),
		{}
		virtual bool check(const Event& e0) const = 0;
		virtual const Event& transform(const Event& e) = 0;
}

class Trans2 : public Trans{
	protected:
		const EventType mInType0;
		const EventType mInType1;
	public:
		Trans2(EventType out, EventType in0, EventType in1)
			: Trans(out, 2),
			  mInType0(in0),
			  mInType1(in1)
		{}
		virtual bool check0(const Event& e) const = 0;
		virtual bool check1(const Event& e) const = 0;
		virtual bool check(const Event& e0, const Event& e1) const = 0;
		virtual const Event& transform(const Event& e0, const Event& e1) = 0;
};

class PosToSpeed : public Trans2{
	private:
		SpeedEvent buffer;
	public:
		PosToSpeed() : 
			Trans2(EventType::Speed, EventType::Position, EventType::Position)
		{}
		virtual bool check0(const Event& e) const{
			return e.type == mInType0;
		}
		virtual bool check1(const Event& e) const{
			return e.type == mInType1;
		}
		virtual bool check(const Event& e0, const Event& e1) const{
			return e1.time > e0.time && e0.objectID == e1.objectID;
		}
		virtual const Event& transform(const Event& e0, const Event& e1){
			buffer.objectID = e0.objectID;
			buffer.pos[0] = (e0.pos[0]+e1.pos[0])/2;
			buffer.pos[1] = (e0.pos[1]+e1.pos[1])/2;
			buffer.time = (e0.time+e1.time)/2;
			float speedX = (e1.pos[0]-e0.pos[0])/(e1.time-e0.time);
			float speedY = (e1.pos[1]-e0.pos[1])/(e1.time-e0.time);
			buffer.speed = sqrt(speedX*speedX + speedY*speedY);
			return buffer;
		}
};

class ProxToPose : public Trans1{
	private:
		PoseEvent buffer;
	public:
		ProxToPose() : 
			Trans1(EventType::Position, EventType::Proximity)
		{}
		virtual bool check(const Event& e) const{
			return e.type == mInType;
		}
		virtual const Event& transform(const Event& e){
			buffer.objectID = e0.secondObjectID;
			buffer.pose = (e0.pos[0]+e1.pos[0])/2;
			buffer.time = e0.time;
			return buffer;
		}
};

ostream& operator<<(ostream& o, const Event& e){
	return e.print(o);
}

int main(){
	PositionEvent p0(0, {0,1}, 10);
	PositionEvent p1(0, {5,3}, 100);
	Event& e0 = p0;
	Event& e1 = p1;
	PosToSpeed pts; 
	Trans2& t= pts;
	cout << e0 << endl;
	cout << e1 << endl;
	cout << t.transform(e0, e1) << endl;
	return 0;
}
