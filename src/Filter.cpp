#include <Filter.h>

#include <boost/phoenix/operator.hpp>

using boost::phoenix::function;

function<filter::uncertainty_impl> filter::uncertainty;
function<filter::value_impl> filter::value;
function<filter::norm_impl> filter::norm;
