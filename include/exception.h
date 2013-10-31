#pragma once

#include <exception>

class AttributeException : public std::exception
{
  public:
    enum class Cause : unsigned int
    {
      typeMismatch,
      scaleNotEqual,
      unitNotEqual,
      lengthNotEqual,
      idNotEqual
    };

  private:
    Cause c;

  public:

    AttributeException(Cause c) noexcept : c(c){}
    virtual const char* what() const noexcept
    {
      switch(c)
      {
        case(Cause::typeMismatch): return "Data types do not match";
        case(Cause::scaleNotEqual): return "Data not scaled equally";
        case(Cause::unitNotEqual): return "Data has different unit then expected";
        case(Cause::lengthNotEqual): return "Vector data has different length";
        case(Cause::idNotEqual): return "Attribute id does not match";
        default: return "Cause unknown";
      }
    }

    const Cause cause() const noexcept{return c;}
};

class TypeException : public AttributeException
{
  public:
    TypeException() noexcept : AttributeException(Cause::typeMismatch){}
};

class ScaleException : public AttributeException
{
  public:
    ScaleException() noexcept : AttributeException(Cause::scaleNotEqual){}
};

class UnitException : public AttributeException
{
  public:
    UnitException() noexcept : AttributeException(Cause::unitNotEqual){}
};

class LengthException : public AttributeException
{
  public:
    LengthException() noexcept : AttributeException(Cause::lengthNotEqual){}
};

class IDException : public AttributeException
{
  public:
    IDException() noexcept : AttributeException(Cause::idNotEqual){}
};
