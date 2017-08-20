#pragma once

#include <Filter.h>
#include <DeSerializer.h>
#include <MetaEvent.h>
#include <MetaFactory.h>

#include <stdexcept>
#include <vector>

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
		/** \brief EventPlaceholders to store events taking part in comparision **/
		EventPlaceholder mE0, mE1;
		/** \brief the comparision operation **/
		FilterOp mOp;
		/** \brief possible constant argument of the comparision **/
		MetaAttribute mAttr;
		/** \brief event type information for deserialization **/
		const std::vector<const EventType*>* mTypesPtr;
	public:
		/** \brief Construct a dynamic filter subexpression container
		 *  \param types a vector containing the subscribed event types
		 *
		 *  The constructor stores a reference to the types of the subscription to enable a correct deserialization of constants used in the filter predicate
		 **/
		MetaPredicate(const std::vector<const EventType*>& types) : mTypesPtr(&types) {}
		/** \brief execute contained filter predicate
		 *
		 *  \param events vector of events the filter predicate may operate on
		 *  \return true on fullfilled subexpression, false otherwise
		 *
		 *  Executea a preveuisly received filter subexpression and return the results
		 **/
		bool operator()(const std::vector<const MetaEvent*>& events) const;
	/** \brief friend declaration of deserialization function **/
	template<typename T> friend DeSerializer<T>& operator>>(DeSerializer<T>&, MetaPredicate&);
	/** \brief friend declaration of output stream operator **/
	friend std::ostream& operator<<(std::ostream&, const MetaPredicate&);
};

/** \brief overloaded output stream operator
 *  \param o the output stream object
 *  \param f the MetaPredicate to be printed
 *  \return the output stream containing the printed MetaPredicate
 **/
std::ostream& operator<<(std::ostream& o, const MetaPredicate& f);
 /** \brief Dynamic filter filled by a filter expression. 
	* 
	* These expressions may be transmitted in subscriptions and allow the publisher to filter events before transmission. They are stated by the subscriber, serialized into the subscription event and deserialized into this class.
	**/
class MetaFilter {
	private:
		/** \brief the filter expression as list of predicate, logical op pairs **/
		std::vector<std::pair<MetaPredicate, id::filterOp::ID>> mExpr;
		/** \brief local definition of noop operation to indicate end of list **/
		static const id::filterOp::ID noop = id::filterOp::NOOP::value;
		/** \brief event type information to handle deserialization **/
		std::vector<const EventType*> mTypes;
	public:
		/** \brief Construct an always true filter **/
    MetaFilter() {}
		/** \brief Construct a dynamic filter container
		 *  \param types a vector containing the subscribed event types
		 *
		 *  The constructor stores a reference to the types of the subscription to enable a correct deserialization of constants used in the filter expressions
		 **/
		MetaFilter(const std::vector<const EventType*>& types) : mTypes(types) {}
		/** \brief execute contained filter
		 *
		 *  \param events vector of events the filter operates on
		 *  \return true on fullfilled expression, false otherwise
		 *
		 *  Executea a preveuisly received filter expression and return the results
		 **/
		bool operator()(const std::vector<const MetaEvent*>& events) const;
	/** \brief friend declaration of deserialization function **/
	template<typename T> friend DeSerializer<T>& operator>>(DeSerializer<T>&, MetaFilter&);
	/** \brief friend declaration of output stream operator **/
	friend std::ostream& operator<<(std::ostream&, const MetaFilter&);
};

/** \brief overloaded output stream operator
 *  \param o the output stream object
 *  \param f the MetaFilter to be printed
 *  \return the output stream containing the printed MetaFilter
 **/
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
        auto attrTPtr = p.mTypesPtr->at(p.mE0.num)->attribute(p.mE0.attr);
			if(!attrTPtr)
				throw MetaFilterError();
			p.mAttr = MetaAttribute(*attrTPtr);
			d >> p.mAttr;
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
  if(f.mTypes.empty())
    return d;
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
