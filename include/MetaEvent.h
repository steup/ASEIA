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
		MetaEvent(const EventType& eT);
		MetaAttribute* attribute(ID id);
		const MetaAttribute* attribute(ID id) const;
    bool add(const MetaAttribute&);
    bool add(MetaAttribute&& mA);
    bool remove(ID id) { return mStorage.erase(id); }

    bool operator==(const MetaEvent& a) const;
    bool operator!=(const MetaEvent& a) const { return !(*this==a); }

    explicit operator EventType() const;

		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		iterator end() noexcept;
		const_iterator end() const noexcept;

  template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, const MetaValue&);
};

std::ostream& operator<<(std::ostream& o, const MetaEvent& me);

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaEvent& me){
	for(const MetaAttribute& attr : me)
		s << attr;
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, MetaEvent& me){
	for(MetaAttribute& attr : me)
		d >> attr;
  return d;
}
