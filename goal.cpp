using EventConversion=DynamicEvent()(const DynamicEvent&);

class EventType{
  private:
    std::vector<AttributeTypes> attributes;
  public:
    EventConversion operator-(EventType& e);
};

class AttributeType{
  private:
    AttributeID id;
    ValueType value;
    Unit unit;
  public:
    AttributeConversion operator-(AttributeType& e);
    AttributeConversion operator*(AttributeType& e);
    AttributeConversion operator/(AttributeType& e);
};

class AttributeID{
  public:
    IDConversion
};

class ValueType{};

class UnitType{
  private:
    Scale s;
};

class ScaleType{};

template<typename Attributes...>
class Event{
  private:
    AttrTuple data;
  public:
    Event();
    EventType getType();
};

template<typename Value, typename Unit>
class Attribute{
  private:
    Value value;
  public:
    AttributeType getType();
};

template<typename T, std::size_t n, bool uncertainty>
class Value{
  public:
    ValueType getType();
};

class DynamicEvent{
  private:
    std::vector<DynamicAttribute> attributes;
  public:
    template<Attribute...>
    DynamicEvent(const Event<Attribute...>&);
};

class DynamicAttribute{
  private:
    DynamicValue value;
    DynamicUnit  unit;
  public:
    template<Attribute>
    DynamicAttribute(const Attribute&);
};

class DynamicValue{
  private:
    std::vector<DynamicValueElement>
  public:
      template<typename Value>
      DynamicValue(const Value&);
};

class DynamicValueElement{
  private:
    uint8_t type;
    union{
      uint8_t uint8;
      uint16_t uint16;
      uint32_t uint32;
      uint64_t uint64;
      int8_t  int8;
      int16_t int16;
      int32_t int32;
      int64_t int64;
      float   f;
      double  d;
    };
  public:
    template<typename T>
    DynamicValueElement(T t);
};
