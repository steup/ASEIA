#pragma once

#include <type_traits>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/distance.hpp>

#include <sensorEvent.h>

template<typename... Attrs>
class SensorEventScheme
{
  private:
    SensorEventScheme() = default;
    template<typename ID>
    struct AttrIDCompare
    {
      template<typename T>
      struct apply
      {
        using type = typename std::is_same<ID, typename T::ID>::type;
      };
    };

  public:
    using AttrList = typename boost::mpl::vector<Attrs...>::type;

    using EventType = SensorEvent<SensorEventScheme>;
    EventType createEvent() const {return EventType();}

    template<typename ID>
    struct attribute
    {
      using compare    =          AttrIDCompare<ID>;
      using iter       = typename boost::mpl::find_if<AttrList, compare>::type;
      using begin_iter = typename boost::mpl::begin<AttrList>::type;
      using end_iter   = typename boost::mpl::end<AttrList>::type;
      static_assert(!std::is_same<iter, end_iter>::value, "No such attribute");
      using pos        = typename boost::mpl::distance<begin_iter, iter>::type;
      using type       = typename boost::mpl::deref<iter>::type;
    };

    template<typename NewAttribute>
    using extent = SensorEventScheme<Attrs..., NewAttribute>;

  private:
    template<typename PacketBufferIterator>
    struct serializeFunctor
    {
      PacketBufferIterator& i;
      const SensorEventScheme& s;
      serializeFunctor(PacketBufferIterator& i,const SensorEventScheme& s) : i(i),s(s){}
      template<typename Attribute>
      void operator()(Attribute& a)
      {
        i << a;
      }
    };

    template<typename PacketBufferConstIterator>
    struct deserializeFunctor
    {
      PacketBufferConstIterator& i;
      SensorEventScheme& s;
      deserializeFunctor(PacketBufferConstIterator& i,SensorEventScheme& s) : i(i),s(s){}
      template<typename Attribute>
      void operator()(Attribute& a)
      {
        i >> a;
      }
    };

  template<typename I, typename S> friend I& operator<<(I&,const SensorEventScheme<S>&);
  template<typename I, typename S> friend I& operator>>(I&,SensorEventScheme<S>&);

    
};

template<typename PacketBufferConstIterator, typename AttrList>
PacketBufferConstIterator& operator>>(PacketBufferConstIterator& i, SensorEventScheme<AttrList>& s)
{
  boost::mpl::for_each<AttrList>(typename SensorEventScheme<AttrList>::template deserializeFunctor<PacketBufferConstIterator>(i,s));
  return i;
}

template<typename PacketBufferIterator, typename AttrList>
PacketBufferIterator& operator<<(PacketBufferIterator& i, const SensorEventScheme<AttrList>& s)
{
  boost::mpl::for_each<AttrList>(typename SensorEventScheme<AttrList>::template serializeFunctor<PacketBufferIterator>(i,s));
  return i;
}
