#pragma once

#include <MetaValueBaseImplementation.h>
#include <ID.h>
#include <Serializer.h>
#include <DeSerializer.h>

#include <iosfwd>
#include <memory>

class MetaScale;
class ValueType;

class MetaValue {
  public:
    using Ptr = MetaValueBaseImplementation::Ptr;
		using ID  = id::type::ID;

	private:
    Ptr mImpl;

	public:
    explicit MetaValue(Ptr&& ref);
    Ptr& implementation() { return mImpl; }
    const Ptr& implementation() const { return mImpl; }
    
    bool resize(std::size_t rows, std::size_t cols);
    bool hasUncertainy(bool u);

  public:
    MetaValue();

    MetaValue(const MetaValue& copy);
    MetaValue(MetaValue&& copy);

    MetaValue& operator=(const MetaValue& b);
    MetaValue& operator=(MetaValue&& b);

    MetaValue operator+(const MetaValue& b) const;
		MetaValue operator==(const MetaValue& b) const;
		MetaValue operator!=(const MetaValue& b) const;
		MetaValue& operator*=(const MetaScale& b);
    /*void set(std::size_t row, std::size_t col, ValueElement<double> value) {
      mImpl->set(row, col, value);
    }*/
    
    std::size_t size()   const;
    std::size_t cols()   const;
    std::size_t rows()   const;
    ID          typeId() const;
    bool hasUncertainty() const;

    bool valid() const;
    bool compatible(const MetaValue& b) const;

    explicit operator ValueType() const;

    std::ostream& print(std::ostream& o) const;

  template<typename PB> friend DeSerializer<PB>& operator>>(DeSerializer<PB>&, const MetaValue&);
  friend class MetaFactoryImplementation;
};

inline std::ostream& operator<<(std::ostream& o, const MetaValue& v) {
  return v.print(o);
}

template<typename PB>
Serializer<PB>& operator<<(Serializer<PB>& s, const MetaValue& me){
  return s;
}

template<typename PB>
DeSerializer<PB>& operator>>(DeSerializer<PB>& d, MetaValue& me){
  return d;
}
