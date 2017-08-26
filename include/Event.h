#pragma once

#include <Endianess.h>
#include <Serializer.h>
#include <DeSerializer.h>
#include <EventType.h>
#include <EventID.h>

#include <type_traits>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/sort.hpp>


template<Endianess e=hostEndianess, typename... Attributes>
class Event : public Attributes...
{
  public:
    static const Endianess endianess = e;
    using AttributeList=boost::mpl::vector<Attributes...>;

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

	private:
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

    template<typename Serializer>
    struct SerializationHelper{
      Serializer& s;
      const Event& ev;
      SerializationHelper(Serializer& s, const Event& ev) : s(s), ev(ev){}
      template<typename Attr>
      void operator()(Attr a){
          s << ev.attribute(a.id());
      }
    };

    template<typename DeSerializer>
    struct DeSerializationHelper{
      DeSerializer& d;
      Event& ev;
      DeSerializationHelper(DeSerializer& d, Event& ev) : d(d), ev(ev){}
      template<typename Attr>
      void operator()(Attr a){
          d >> ev.attribute(a.id());
      }
    };

		struct EventTypeHelper{
        EventType eT;

        template<typename Attr>
        void operator()(Attr attr) {
          eT.add((AttributeType)attr);
        }
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
    auto operator[](ID i) const -> const typename findAttribute<ID>::type&{
      return attribute(i);
    }

    template<typename ID>
    auto attribute(ID i) -> typename findAttribute<ID>::type&{
      using TargetAttribute = typename findAttribute<ID>::type;
      return *static_cast<TargetAttribute*>(this);
    }

    template<typename ID>
    auto operator[](ID i) -> typename findAttribute<ID>::type&{
      return attribute(i);
    }

		explicit operator EventType() const {
			EventTypeHelper eTH;
      foreach<AttributeList>(eTH);
			return eTH.eT;
    }

		EventID id() const { return EventID(*this); }

    static constexpr std::size_t size() noexcept{
      using namespace boost::mpl;
      using sizes = typename transform<AttributeList, toConstInt>::type;
      using sum   = typename fold<sizes, int_<0>, addConstInt>::type;
      return sum::value;
    }

  template<typename PB, Endianess end,typename... Attrs> friend Serializer<PB>& operator<<(Serializer<PB>&, const Event<end, Attrs...>&);
  template<typename PB, Endianess end,typename... Attrs> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, Event<end, Attrs...>&);
};

struct AttributeLess {
  template<typename A, typename B>
  struct apply {
    using type = boost::mpl::bool_< A::IDType::value() < B::IDType::value() >;
  };
};

template<typename PB, Endianess end,typename... Attrs>
Serializer<PB>& operator<<(Serializer<PB>& s, const Event<end, Attrs...>& e){
  using E = Event<end, Attrs...>;
  using Helper = typename E::template SerializationHelper<Serializer<PB>>;
  using AttrList = typename boost::mpl::sort<typename E::AttributeList, AttributeLess>::type;
  boost::mpl::for_each<AttrList>(Helper(s,e));
  return s;
}

template<typename PB, Endianess end,typename... Attrs>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, Event<end, Attrs...>& e){
  using E = Event<end, Attrs...>;
  using Helper = typename E::template DeSerializationHelper<DeSerializer<PB>>;
  using AttrList = typename boost::mpl::sort<typename E::AttributeList, AttributeLess>::type;
  boost::mpl::for_each<AttrList>(Helper(d,e));
  return d;
}
