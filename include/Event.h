#pragma once

#include <Endianess.h>
#include <ID.h>

#include <type_traits>
#include <functional>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/lambda.hpp>


template<Endianess e=hostEndianess, typename... Attributes>
class Event : public Attributes...
{
  public:
    using AttributeList=boost::mpl::vector<Attributes...>;

  private:
    template<typename ID>
    struct findAttribute{
      struct compare{
        template<typename Attr>
        struct apply{
          using type=typename std::is_same<ID, typename Attr::IDType>::type;
        };
      };
      using TargetIterator  = typename boost::mpl::find_if<AttributeList, compare>::type;
      using type = typename boost::mpl::deref<TargetIterator>::type;
    };

    struct toConstInt{
      template<typename Attr>
      struct apply{
        using type = boost::mpl::int_<Attr::size()>;
      };
    };

    struct addConstInt{
      template<typename Sum, typename V>
      struct apply{
        using type = boost::mpl::plus<Sum,V>;
      };
    };

  public:
    
    template<typename NewAttribute>
    struct append{
      using type = Event<e, Attributes..., NewAttribute>;
    };

    template<typename ID>
    auto attribute(ID i) const -> const typename findAttribute<ID>::type&{
      using TargetAttribute = typename findAttribute<ID>::type;
      return *static_cast<const TargetAttribute*>(this);
    }

    template<typename ID>
    auto attribute(ID i) -> typename findAttribute<ID>::type&{
      using TargetAttribute = typename findAttribute<ID>::type;
      return *static_cast<TargetAttribute*>(this);
    }

    static constexpr std::size_t size() noexcept{
      using namespace boost::mpl;
      using sizes = typename transform<AttributeList, toConstInt>::type;
      using sum   = typename fold<sizes, int_<0>, addConstInt>::type;
      return sum::value;
    }
};
