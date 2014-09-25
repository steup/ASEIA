#pragma once

#include <new>

template<typename T>
class Singleton : public T {
  private:
    static bool sConstructed;
    static char sBuffer[sizeof(T)];
  protected:
    Singleton() : T() { };
    Singleton& operator=(const Singleton&) = delete;
    Singleton(const Singleton&) = delete;
  public:
    static Singleton& instance() { 
      union {
        char* buffer;
        Singleton* ptr;
      } u = { sBuffer };
      if(!sConstructed) {
        sConstructed = true;
        new(u.buffer) Singleton();
      }
      return *u.ptr;
    }
    ~Singleton() { 
      sConstructed = false;
    }
};

template<typename T>
bool Singleton<T>::sConstructed = false;

template<typename T>
char Singleton<T>::sBuffer[sizeof(T)];
