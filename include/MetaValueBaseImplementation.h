#pragma once

#include <ValueElement.h>
#include <ID.h>

#include <memory>
#include <iosfwd>
#include <cstdint>

class MetaScale;

class MetaValueBaseImplementation {
  protected:
    using Interface = MetaValueBaseImplementation;
    static Interface sInstance;

    struct Deleter{
      void operator()(Interface* ptr){
        if( ptr != &sInstance )
          delete ptr;
      }
    } deleter;

    virtual void resize( std::size_t rows, std::size_t cols) { }
    virtual void hasUncertainty( bool u ) { }

    MetaValueBaseImplementation() = default;

  public:
    using Ptr = std::unique_ptr<Interface, Deleter>;
    virtual ~MetaValueBaseImplementation() = default;

    virtual Interface& operator=( const Interface& b) { 
      return *this; 
    }

    virtual Ptr copy() const { 
      return Ptr(&sInstance, sInstance.deleter);
    }

    virtual Interface& operator+=( const Interface& b ) {
      return *this; 
    }

		virtual Interface& operator*=(const MetaScale& b) { return *this; }

    virtual id::type::ID typeId() const { 
      return id::type::Base::value(); 
    }
    
    virtual void set(std::size_t row, std::size_t col, ValueElement<double> value) { }

    virtual std::size_t cols() const { 
      return 0;
    }

    virtual std::size_t rows() const { 
      return 0;
    }

    virtual bool hasUncertainty() const { 
      return true; 
    }

    virtual std::size_t size() const { 
      return 0; 
    }

    virtual std::ostream& print( std::ostream& o ) const;

    friend class MetaFactoryImplementation;
    friend class MetaValue;
};

std::ostream& operator<<(std::ostream& o, const MetaValueBaseImplementation& mvbi);
