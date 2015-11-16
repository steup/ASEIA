#pragma once

#include <ID.h>
#include <MetaAttribute.h>
#include <Serializer.h>
#include <DeSerializer.h>

#include <map>
#include <ostream>

class MetaEvent{
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
		MetaAttribute& attribute(id::attribute::ID id);

		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		iterator end() noexcept;
		const_iterator end() const noexcept;

  template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, const MetaValue&);
};

std::ostream& operator<<(std::ostream& o, const MetaEvent& me);

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaEvent& me){
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, MetaEvent& me){
  return d;
}
