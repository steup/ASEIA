#pragma once

#include <Filter.h>
#include <DeSerializer.h>
#include <MetaEvent.h>
#include <MetaFactory.h>
#include <stdexcept>

/** \brief Dynamic filter error class
 *  \todo add cause enumeration
 **/
class MetaFilterError : public std::runtime_error {
	public:
		MetaFilterError() : std::runtime_error("MetaFilterError") {}
};

/** \brief A single predicate of a received filter expression
 *
 *  This class describes single precidates evaluated as part of a filter express contained in a MetaFilter.
 **/
class MetaPredicate {
	private:
		EventPlaceholder mE0, mE1;
		FilterOp mOp;
		MetaValue mV;
		const std::vector<EventType>& mTypes;
	public:
		/** \brief Construct a dynamic filter subexpression container
		 *  \param types a vector containing the subscribed event types
		 *
		 *  The constructor stores a reference to the types of the subscription to enable a correct deserialization of constants used in the filter predicate
		 **/
		MetaPredicate(const std::vector<EventType>& types) : mTypes(types) {}
		/** \brief execute contained filter predicate
		 *
		 *  \param events vector of events the filter predicate may operate on
		 *  \return true on fullfilled subexpression, false otherwise
		 *
		 *  Executea a preveuisly received filter subexpression and return the results
		 **/
		bool operator()(const std::vector<MetaEvent>& events) const;
	/** \brief friend declaration of deserialization function **/
	template<typename T> friend DeSerializer<T>& operator>>(DeSerializer<T>&, MetaPredicate&);
	friend std::ostream& operator<<(std::ostream&, const MetaPredicate&);
};

std::ostream& operator<<(std::ostream& o, const MetaPredicate& f);
 /** \brief Dynamic filter filled by a filter expression. 
	* 
	* These expressions may be transmitted in subscriptions and allow the publisher to filter events before transmission. They are stated by the subscriber, serialized into the subscription event and deserialized into this class.
	**/
class MetaFilter {
	private:
		std::vector<std::pair<MetaPredicate, id::filterOp::ID>> mExpr;
		static const id::filterOp::ID noop = id::filterOp::NOOP::value;
		const std::vector<EventType>& mTypes;
	public:
		/** \brief Construct a dynamic filter container
		 *  \param types a vector containing the subscribed event types
		 *
		 *  The constructor stores a reference to the types of the subscription to enable a correct deserialization of constants used in the filter expressions
		 **/
		MetaFilter(const std::vector<EventType>& types) : mTypes(types) {}
		/** \brief execute contained filter
		 *
		 *  \param events vector of events the filter operates on
		 *  \return true on fullfilled expression, false otherwise
		 *
		 *  Executea a preveuisly received filter expression and return the results
		 **/
		bool operator()(const std::vector<MetaEvent>& events) const;
	/** \brief friend declaration of deserialization function **/
	template<typename T> friend DeSerializer<T>& operator>>(DeSerializer<T>&, MetaFilter&);
	friend std::ostream& operator<<(std::ostream&, const MetaFilter&);
};

std::ostream& operator<<(std::ostream& o, const MetaFilter& f);


/** \brief overloaded and templated deserialization function for dynamic filter predicates
 *  \tparam T the iterator type of the DeSerializer
 *  \param d the used DeSerializer object
 *  \param p the MetaPredicate to store the filter expression part to
 *  \return the DeSerializer moved forward by the size of the extracted filter predicate
 **/
template<typename T>
DeSerializer<T>& operator>>(DeSerializer<T>& d, MetaPredicate& p) {
	d >> p.mE0.data >> p.mOp.data;
	try {
		if(p.mOp.constArg) {
			auto attrTPtr = p.mTypes.at(p.mE0.num).attribute(p.mE0.attr);
			if(!attrTPtr)
				throw MetaFilterError();
			ValueType vt = attrTPtr->value();
			p.mV = MetaFactory::instance().create(vt);
			d >> p.mV;
		} else
			d >> p.mE1.data;
	}
	catch(const MetaFilterError& e) { 
		throw e; 
	}
	catch(const std::exception& e) { 
		throw MetaFilterError(); 
	}
	if(d.error())
		throw MetaFilterError();
	return d;
}

/** \brief overloaded and templated deserialization function for dynamic filter expression
 *  \tparam T the iterator type of the DeSerializer
 *  \param d the used DeSerializer object
 *  \param p the MetaFilter to store the filter expression
 *  \return the DeSerializer moved forward by the size of the extracted filter
 **/
template<typename T>
DeSerializer<T>& operator>>(DeSerializer<T>& d, MetaFilter& f) {
	id::filterOp::ID logicalOp;
	do{
		MetaPredicate p(f.mTypes);
		d >> p >> logicalOp;
		f.mExpr.emplace_back(p, logicalOp);
	}while(logicalOp != id::filterOp::NOOP::value && !d.error());
	if(d.error())
		throw MetaFilterError();
	return d;
}
