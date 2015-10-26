#pragma once

#include <MetaScale.h>

#include <cstdint>
#include <ID.h>
#include <ostream>
#include <memory>

class MetaValueBaseImplementation {
  private:
    using Interface = MetaValueBaseImplementation;
  protected:
    static Interface sInstance;

    struct Deleter{
      void operator()(Type* ptr){
        if( ptr != &sInstance )
          delete ptr;
      }
    } deleter;

    virtual void rows( std::size_t rows) { }
    virtual void cols( std::size_t rows) { }
    virtual void uncertain( bool u ) { }

    MetaValueBaseImplementation() = default;

  public:
    using Ptr = std::unique_ptr<Type, Deleter>;
    virtual ~MetaValueBaseImplementation() = default;

    virtual Interface& operator=( const Type& b) { 
      return *this; 
    }

    virtual Ptr copy() const { 
      return Ptr(&sInstance, sInstance.deleter);
    }

    virtual Interface& operator+=( const Type& b ) {
      return *this; 
    }

		virtual Interface& scale(const MetaScale& b) { }

    virtual id::type::ID typeId() const { 
      return id::type::Base::value(); 
    }
    
    virtual void set(std::size_t rows, std::size_t cols, double value, double uncertainty) { }

    virtual std::size_t cols() const { 
      return 0;
    }

    virtual std::size_t rows() const { 
      return 0;
    }

    virtual bool uncertain() const { 
      return true; 
    }

    virtual std::size_t size() const { 
      return 0; 
    }

    virtual void print( std::ostream& o ) const { 
      o << "void"; 
    }

    friend class MetaFactoryImplementation;
    friend class MetaValue;
};

std::ostream& operator<<(std::ostream& o, const MetaValueBaseImplementation& mvbi);
