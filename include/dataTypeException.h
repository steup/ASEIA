#pragma once

#include <exception>

class DataTypeException : public std::exception
{
  public:
    static DataTypeException incompatibleTypes;

  private:
    const char* const s;

  public:
    DataTypeException(const char* s) : s(s){}
    virtual const char* what() const noexcept{return s;}
};

DataTypeException DataTypeException::incompatibleTypes("Incompatible data types between publisher and subscriber");
