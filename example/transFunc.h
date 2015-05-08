#pragma once

#include "transEvent.h"

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
		Trans1(EventType out, EventType in)
			: Trans(out, 1),
			  mInType(in)
		{}
		virtual bool check(const Event& e0) const = 0;
		virtual const Event& transform(const Event& e) = 0;
};

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
			buffer.pose.pos = F(0.5)*(e0.pose.pos+e1.pose.pos);
			buffer.pose.theta = F(0.5)*(e0.pose.theta+e1.pose.theta);
			buffer.time = F(0.5)*(e0.time+e1.time);
      auto dV = e1.pose.pos-e0.pose.pos;
			buffer.speed.d = norm_2(dV)/(e1.time-e0.time);
      buffer.speed.theta = atan2(dV(1),dV(0));
			return buffer;
		}
};

class ProxToPose : public Trans1{
	private:
		PositionEvent buffer;
	public:
		ProxToPose() : 
			Trans1(EventType::Position, EventType::Proximity)
		{}
		virtual bool check(const Event& e) const{
			return e.type == mInType;
		}
		virtual const Event& transform(const Event& e){
      const ProximityEvent& pe = dynamic_cast<const ProximityEvent&>(e);
			buffer.objectID = pe.secondObject;
			buffer.time = pe.time;
//      buffer.pose.pos(0) = pe.pose.pos(0)+boost::numeric::cos(pe.distance.theta)*pe.distance.d;
//      buffer.pose.pos(1) = pe.pose.pos(1)+boost::numeric::sin(pe.distance.theta)*pe.distance.d;
			return buffer;
		}
};
