#pragma once

#include <MetaScale.h>

#include <cstdint>
#include <ID.h>
#include <ostream>
#include <memory>

class MetaValueBaseImplementation {
  private:
    using Type = MetaValueBaseImplementation;
  protected:
    static  Type sInstance;

    struct Deleter{
      void operator()(Type* ptr){
        if( ptr != &sInstance )
          delete ptr;
      }
    } deleter;

    virtual void n( std::size_t n) { }
    virtual void hasUncetrainty( bool u ) { }

    MetaValueBaseImplementation() = default;

  public:
    using Ptr = std::unique_ptr<Type, Deleter>;
    virtual ~MetaValueBaseImplementation() = default;

    virtual Type& operator=( const Type& b) { 
      return *this; 
    }

    virtual Ptr copy() const { 
      return Ptr(&sInstance, sInstance.deleter);
    }

    virtual Type& operator+=( const Type& b ) {
      return *this; 
    }

		virtual void scale(const MetaScale& b) { }

    virtual id::type::ID typeId() const { 
      return id::type::Base::value(); 
    }
    
    virtual void set(uint8_t i, double value, double uncertainty) { }

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
