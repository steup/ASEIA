#pragma once

#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>

namespace {
  template<typename I, typename End, typename RuntimeOp>
  struct Foreach{
    using Element = typename boost::mpl::deref<I>::type;
    using next = typename boost::mpl::next<I>::type;
    void operator()(RuntimeOp& op) {
      Element temp;
      op(temp);
      Foreach<next, End, RuntimeOp>()(op);
    }
  };  

  template<typename End, typename RuntimeOp>
  struct Foreach<End, End, RuntimeOp>{
    void operator()(RuntimeOp& op) {}
  }; 
}

template<typename Sequence, typename RuntimeOp>
void foreach(RuntimeOp& op){
  using begin = typename boost::mpl::begin<Sequence>::type;
  using end   = typename boost::mpl::end<Sequence>::type;
  Foreach<begin, end, RuntimeOp>()(op);
}
