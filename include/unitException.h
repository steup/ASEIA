#pragma once

#include <exception>

class UnitException : public std::exception
{
  public:
    static UnitException incompatibleUnits;
    static UnitException scalesNotEqual;

  private:
    const char* const s;

  protected:
    UnitException(const char* s) noexcept : s(s){};

  public:
    virtual const char* what() const noexcept {return s;}
};

UnitException UnitException::incompatibleUnits("Incompatible units in operation");
UnitException UnitException::scalesNotEqual("Scales not equal");
