#pragma once

#include <EventID.h>
#include <Transformation.h>

#include <cstdint>
#include <list>

template<typename T>
class Singleton {
  protected:
    Singleton() = default;
  private:
    static bool sConstructed;
    static std::uint8_t sBuffer[sizeof(T)];
  public:
    static T& instance() {
      if(!sConstructed) {
        new(sBuffer) T();
        sConstructed = true;
      }
      return *reinterpret_cast<T*>(sBuffer);
    }
};

class TransformIterator
  : std::iterator<
    std::input_iterator_tag,
    TransPtr>
{
  friend class TransformGenerator;
  private:
    TransformGenerator& mGen;
    std::list<const Transformation*> mTrans;
    TransformIterator(TransformGenerator& tG) = default;
  public:
    TransPtr operator*() const;
    TransformIterator operator++() const;
    bool operator==(const TransformIterator& b) const;
};


class TransformGenerator {
  friend class KnowledgeBase;
  private:
    using EventIDs = std::list<EventID>;
    EventID out;
    EventIDs in;
    TransformGenerator(EventID out, const EventIDs& in);
  public:
    TransformIterator begin();
    TransformIterator end();
};

class KnowledgeBase : public Singleton<KnowledgeBase> {
  protected:
    KnowledgeBase() = default;
  public:
    using EventIDs = std::list<EventID>;
    void registerTransformation(const Transformation& trans);
    void unregisterTransformation(const Transformation& trans);
    TransformGenerator generate(EventID out, EventIDs in) const;
};
