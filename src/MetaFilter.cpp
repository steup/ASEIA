#include <MetaFilter.h>
#include <IDIO.h>
using namespace ::id::filterOp;

static const MetaAttribute& extractAttr(EventPlaceholder e, const std::vector<const MetaEvent*> events) {
	const MetaAttribute* aPtr = events[e.num]->attribute(e.attr);
  static const MetaAttribute invalid;
	return !aPtr?invalid:*aPtr;
}

bool MetaPredicate::operator()(const std::vector<const MetaEvent*>& events) const {
  MetaAttribute a = extractAttr(mE0, events);
  for(auto func: mUnaryFuncs)
    a=(a.*func)();
	const MetaAttribute& b = !mOp.constArg?extractAttr(mE1, events):mAttr;
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

std::ostream& operator<<(std::ostream& o, const MetaPredicate& p){
	o << "e" << (uint16_t)p.mE0.num << "[" << id::attribute::name(p.mE0.attr) << "]";
  for(auto func : p.func()) {
    if(func == &MetaAttribute::uncertainty)
      o << ".uncertainty()";
    if(func == &MetaAttribute::certain)
      o << ".certain()";
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
		o << "e" << (uint16_t)p.mE1.num << "[" << id::attribute::name(p.mE1.attr) << "]";
	return o;
}

bool MetaFilter::operator()(const std::vector<const MetaEvent*>& events) const {
  if(mTypes.empty())
    return true;
	bool result=true;
	ID op = NOOP::value;
	for(const auto& subExpr : mExpr) {
		bool temp = subExpr.first(events);
		switch(op){
			case(AND::value):  result = temp && result;
								  break;
			case(OR::value):   result = result || temp;
								  break;
			case(NOOP::value): result = temp;
												 break;
			default:    return false;
		}
		op = subExpr.second;
	}
	return result;
}

std::ostream& operator<<(std::ostream& o, const MetaFilter& f){
	o  << "Filter: ";
	for(const auto& p : f.mExpr) {
		o << p.first << " ";
		if(p.second != MetaFilter::noop)
			o << p.second << " ";
	}
	return o;
}
