#include <MetaFilter.h>
#include <IDIO.h>
using namespace ::id::filterOp;

bool MetaPredicate::operator()(const std::vector<MetaEvent>& events) const {
	auto aPtr = events.at(mE0.num).attribute(mE0.attr);
	if(!aPtr)
		return false;

	const MetaValue& a = aPtr->value();
	const MetaValue* b = &mV;
	if(!mOp.constArg) {
		const MetaAttribute* bPtr = events.at(mE1.num).attribute(mE1.attr);
		if(!bPtr)
			return false;
		b = &bPtr->value();
	}
	switch(mOp.code){
		case(LE::value): return (bool)(a <= *b).prod();
		case(GE::value): return (bool)(a >= *b).prod();
		case(LT::value): return (bool)(a  < *b).prod();
		case(GT::value): return (bool)(a  > *b).prod();
		case(EQ::value): return (bool)(a == *b).prod();
		case(NE::value): return (bool)(a != *b).sum();
		default:
			return false;
	}
}

std::ostream& operator<<(std::ostream& o, const MetaPredicate& p){
	o << "e" << (uint16_t)p.mE0.num << "[" << id::attribute::name(p.mE0.attr) << "]";
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
		o << p.mV;
	else
		o << "e" << (uint16_t)p.mE1.num << "[" << id::attribute::name(p.mE1.attr) << "]";
	return o;
}

bool MetaFilter::operator()(const std::vector<MetaEvent>& events) const {
	bool result;
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