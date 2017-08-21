#include <Filter.h>

#include <boost/phoenix/operator.hpp>

using boost::phoenix::function;

function<filter::uncertainty_impl> filter::uncertainty;
function<filter::certain_impl> filter::certain;
function<filter::norm_impl> filter::norm;
