#pragma once

#include <ostream>
#include <tuple>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/for_each.hpp>

template<typename SchemeDefinition>
class SensorEvent
{
  public:
    using Scheme = SchemeDefinition;

  private:
    SensorEvent() = default;

    struct concat
    {
      template<typename tuple, typename argument>
      struct apply
      {
        using tupArg = std::tuple<argument>;
        using type  =  decltype(std::tuple_cat(tuple(), tupArg()));
      };
    };

    using AttrTuple = typename boost::mpl::fold<typename Scheme::AttrList, std::tuple<>, concat>::type;

  private:
    AttrTuple data;

  public:
    template<typename ID>
    const typename Scheme::template attribute<ID>::type& attribute() const
    {
      using n = typename Scheme::template attribute<ID>::pos;
      return std::get<n::value>(data);
    }

    template<typename ID>
    typename Scheme::template attribute<ID>::type& attribute()
    {
      using n = typename Scheme::template attribute<ID>::pos;
      return std::get<n::value>(data);
    }

  private:
    struct ostreamFunctor
    {
      std::ostream& o;
      const SensorEvent& e;
      ostreamFunctor(std::ostream& o,const SensorEvent& e) : o(o),e(e){}
      template<typename Attribute>
      void operator()(Attribute& a)
      {
        o << "\t" << e.template attribute<typename Attribute::ID>() << std::endl;
      }
    };

    template<typename PacketBufferIterator>
    struct serializeFunctor
    {
      PacketBufferIterator& i;
      const SensorEvent& e;
      serializeFunctor(PacketBufferIterator& i,const SensorEvent& e) : i(i),e(e){}
      template<typename Attribute>
      void operator()(Attribute& a)
      {
        i << e.template attribute<typename Attribute::ID>().value();
      }
    };

    template<typename PacketBufferConstIterator>
    struct deserializeFunctor
    {
      PacketBufferConstIterator& i;
      SensorEvent& e;
      deserializeFunctor(PacketBufferConstIterator& i,SensorEvent& e) : i(i),e(e){}
      template<typename Attribute>
      void operator()(Attribute& a)
      {
        i >> e.template attribute<typename Attribute::ID>().value();
      }
    };

  public:
  template<typename...> friend class SensorEventScheme;

  template<typename S> friend std::ostream& operator<<(std::ostream&,const SensorEvent<S>&);
  template<typename I, typename S> friend I& operator<<(I&,const SensorEvent<S>&);
  template<typename I, typename S> friend I& operator>>(I&,SensorEvent<S>&);


};

template<typename Scheme>
std::ostream& operator<<(std::ostream& o, const SensorEvent<Scheme>& e)
{
  o << "Sensor Event: " << std::endl;
  boost::mpl::for_each<typename Scheme::AttrList>(typename SensorEvent<Scheme>::ostreamFunctor(o,e));
  return o;
}

template<typename PacketBufferConstIterator, typename Scheme>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& i, SensorEvent<Scheme>& e)
{
  boost::mpl::for_each<typename Scheme::AttrList>(typename SensorEvent<Scheme>::template deserializeFunctor<PacketBufferConstIterator>(i,e));
  return i;
}

template<typename PacketBufferIterator, typename Scheme>
PacketBufferIterator& operator<<(PacketBufferIterator& i, const SensorEvent<Scheme>& e)
{
  boost::mpl::for_each<typename Scheme::AttrList>(typename SensorEvent<Scheme>::template serializeFunctor<PacketBufferIterator>(i,e));
  return i;
}
