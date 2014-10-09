#pragma once

#include <cstdint>
#include <ID.h>
#include <ostream>

class MetaValueBaseImplementation {
  private:
    using Type = MetaValueBaseImplementation;
  protected:
    static  Type sInstance;

    virtual void n( std::size_t n) { }
    virtual void hasUncetrainty( bool u ) { }

    MetaValueBaseImplementation() = default;

  public:
    virtual ~MetaValueBaseImplementation() = default;

    virtual Type& operator=( const Type& b) { 
      return *this; 
    }

    virtual Type& copy() const { 
      return sInstance; 
    }

    virtual Type& operator+=( const Type& b ) {
      return *this; 
    }

    virtual id::type::ID typeId() const { 
      return id::type::Base::value(); 
    }

    virtual std::size_t n() const { 
      return 0; 
    }

    virtual bool hasUncertainty() const { 
      return false; 
    }

    virtual std::size_t size() const { 
      return sizeof(Type); 
    }

    virtual void print( std::ostream& o ) const { 
      o << "void"; 
    }

    friend class MetaFactoryImplementation;
    friend class MetaValue;
};
