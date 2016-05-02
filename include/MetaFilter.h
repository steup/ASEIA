#pragma once

#include <Filter.h>
#include <DeSerializer.h>
#include <MetaEvent.h>

class MetaPredicate {
	private:
		EventPlaceholder mE0, mE1;
		FilterOp mOp;
		MetaValue mV;
	public:
		bool operator()(const MetaEvent& e) const;
		bool operator()(const MetaEvent& e0, const MetaEvent& e1) const;
		bool constArg() const { return mOp.constArg; }
	template<typename T> friend DeSerializer<T>& operator>>(DeSerializer<T>&, MetaPredicate&);
};

class MetaFilter {
	private:
		std::vector<std::pair<MetaPredicate, id::filterOp::ID>> mExpr;
		static const id::filterOp::ID noop = id::filterOp::NOOP::value;
	public:
		bool operator()(const std::vector<MetaEvent>& events) const;
	template<typename T> friend DeSerializer<T>& operator>>(DeSerializer<T>&, MetaFilter&);
};
	
/** \todo insert deserialization code */
template<typename T>
DeSerializer<T>& operator>>(DeSerializer<T>& d, MetaPredicate& p){
	return d;
}

/** \todo insert deserialization code */
template<typename T>
DeSerializer<T>& operator>>(DeSerializer<T>& d, MetaFilter& f) {
	return d;
}
