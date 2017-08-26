#pragma once

#include <ID.h>
#include <MetaAttribute.h>
#include <EventType.h>
#include <Serializer.h>
#include <DeSerializer.h>

#include <map>
#include <ostream>

class MetaEvent{
  public:
    using ID = id::attribute::ID;
  private:
		using Storage =  std::map<id::attribute::ID, MetaAttribute>;
		Storage mStorage;
		
		class iterator : public Storage::iterator{
			public:
				iterator(Storage::iterator iter);
				MetaAttribute& operator*();
		};
		
		class const_iterator : public Storage::const_iterator{
			public:
				const_iterator(Storage::const_iterator iter);
				const MetaAttribute& operator*() const;
		};

  public:
		MetaEvent() = default;
		explicit MetaEvent(const EventType& eT);
		MetaAttribute* attribute(ID id);
		const MetaAttribute* attribute(ID id) const;
		MetaAttribute& operator[](ID id)             { return *attribute(id); }
		const MetaAttribute& operator[](ID id) const { return *attribute(id); }
    bool add(const MetaAttribute&);
    bool add(MetaAttribute&& mA);
    bool remove(ID id) { return mStorage.erase(id); }

    MetaEvent& operator=(const MetaEvent& copy);
    MetaEvent& operator+=(const MetaEvent& b);
    MetaEvent& operator-=(const MetaEvent& b);
    MetaEvent& operator*=(const MetaValue& b);
    MetaEvent& operator/=(const MetaValue& b);

    bool compatible(const MetaEvent& b) const;
    bool valid() const;

    bool operator==(const MetaEvent& a) const;
    bool operator!=(const MetaEvent& a) const { return !(*this==a); }

    explicit operator EventType() const;
    size_t length() const { return mStorage.size(); }

		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		iterator end() noexcept;
		const_iterator end() const noexcept;

  template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, const MetaValue&);
};

std::ostream& operator<<(std::ostream& o, const MetaEvent& me);

inline MetaEvent operator+(const MetaEvent& a, const MetaEvent& b) { return MetaEvent(a)+=b; }
inline MetaEvent operator-(const MetaEvent& a, const MetaEvent& b) { return MetaEvent(a)-=b; }
inline MetaEvent operator*(const MetaEvent& a, const MetaValue& b) { return MetaEvent(a)*=b; }
inline MetaEvent operator/(const MetaEvent& a, const MetaValue& b) { return MetaEvent(a)/=b; }
MetaEvent operator*(const MetaValue& a, const MetaEvent& b);

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaEvent& me){
  id::attribute::ID attrs[me.length()];
  std::transform(me.begin(), me.end(), attrs, [](const MetaAttribute& mA){ return mA.id(); });
  std::sort(attrs, attrs+me.length());
  for(id::attribute::ID attr : attrs)
		s << *me.attribute(attr);
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, MetaEvent& me){
  id::attribute::ID attrs[me.length()];
  std::transform(me.begin(), me.end(), attrs, [](const MetaAttribute& mA){ return mA.id(); });
  std::sort(attrs, attrs+me.length());
  for(id::attribute::ID attr : attrs)
		d >> *me.attribute(attr);
  return d;
}
