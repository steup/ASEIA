#pragma once

#include <boost/phoenix/core/argument.hpp>
#include <boost/phoenix/operator.hpp>
#include <Serializer.h>

namespace filter {
  const boost::phoenix::expression::argument<1>::type e0 = {};
  const boost::phoenix::expression::argument<2>::type e1 = {};
  const boost::phoenix::expression::argument<3>::type e2 = {};
}

template<typename Iterator>
class FilterSerializer : public Serializer<Iterator>{
	public:
		FilterSerializer(Iterator start) : Serializer<Iterator>(start) {}
	private:
};
