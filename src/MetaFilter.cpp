#include <MetaFilter.h>
#include <IDIO.h>
using namespace ::id::filterOp;

static const MetaAttribute& extractAttr(uint8_t num, uint8_t attr, const std::vector<const MetaEvent*> events) {
	const MetaAttribute* aPtr = events[num]->attribute(attr);
  static const MetaAttribute invalid;
	return !aPtr?invalid:*aPtr;
}

bool MetaPredicate::operator()(const std::vector<const MetaEvent*>& events) const {
  MetaAttribute a = extractAttr(mE0Num, mE0Attr, events);
  for(auto func: mUnaryFuncs)
    a=(a.*func)();
	const MetaAttribute& b = !mOp.constArg?extractAttr(mE1Num, mE1Attr, events):mAttr;
	switch(mOp.code){
		case(LE::value): return (bool)(a <= b).prod();
		case(GE::value): return (bool)(a >= b).prod();
		case(LT::value): return (bool)(a  < b).prod();
		case(GT::value): return (bool)(a  > b).prod();
		case(EQ::value): return (bool)(a == b).prod();
		case(NE::value): return (bool)(a != b).sum();
		default:
			return false;
	}
}

bool MetaPredicate::operator==(const MetaPredicate& b) const {
  if(mE0Num != b.mE0Num || mE0Attr != b.mE0Attr || mOp.code != b.mOp.code)
    return false;
  if(mOp.constArg)
    return (bool)(mAttr == b.mAttr);
  else
    return mE1Num == b.mE1Num && mE1Attr == b.mE1Attr;
}

std::ostream& operator<<(std::ostream& o, const MetaPredicate& p){
	o << "e" << (uint16_t)p.mE0Num << "[" << id::attribute::name(p.mE0Attr) << "]";
  for(auto func : p.func()) {
    if(func == &MetaAttribute::uncertainty)
      o << ".uncertainty()";
    if(func == &MetaAttribute::valueOnly)
      o << ".valueOnly()";
    if(func == &MetaAttribute::norm)
      o << ".norm()";
  }
	switch(p.mOp.code) {
		case(LE::value): o << " <= "; break;
		case(GE::value): o << " >= "; break;
		case(LT::value): o << " <  "; break;
		case(GT::value): o << " >  "; break;
		case(EQ::value): o << " == "; break;
		case(NE::value): o << " != "; break;
		default: o << "unknown";
	}
	if(p.mOp.constArg)
		o << p.mAttr;
	else
		o << "e" << (uint16_t)p.mE1Num << "[" << id::attribute::name(p.mE1Attr) << "]";
	return o;
}

bool MetaFilter::operator()(const std::vector<const MetaEvent*>& events) const {
  if(mTypes.empty())
    return true;
	bool result=true;
	ID op = NOP::value;
	for(const auto& subExpr : mExpr) {
		bool temp = subExpr.first(events);
		switch(op){
			case(AND::value):  result = temp && result;
								  break;
			case(OR::value):   result = result || temp;
								  break;
			case(NOP::value): result = temp;
												 break;
			default:    return false;
		}
		op = subExpr.second;
	}
	return result;
}

bool MetaFilter::operator==(const MetaFilter& b) const {
  auto bIt = b.mExpr.begin();
  if(mExpr.size() != b.mExpr.size())
    return false;
  for(const auto& elem : mExpr) {
    if(elem.second != bIt->second || elem.first != bIt->first)
      return false;
    bIt++;
  }
  return true;
}

std::ostream& operator<<(std::ostream& o, const MetaFilter& f){
	o  << "Filter: ";
	for(const auto& p : f.mExpr) {
		o << p.first << " ";
		if(p.second != NOP())
			o << p.second << " ";
	}
	return o;
}
