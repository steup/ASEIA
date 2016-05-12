#include <MetaFilter.h>

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
		case(LE::value): return a <= *b;
		case(GE::value): return a >= *b;
		case(LT::value): return a < *b;
		case(GT::value): return a > *b;
		case(EQ::value): return a == *b;
		case(NE::value): return a != *b;
		default:
			return false;
	}
}
bool MetaFilter::operator()(const std::vector<MetaEvent>& events) const {
	bool result = true;
	for(const auto& subExpr : mExpr) {
		bool temp = subExpr.first(events);
		switch(subExpr.second){
			case(AND::value):  result = result && temp;
								  break;
			case(OR::value):   result = result || temp;
								  break;
			case(NOOP::value): break;
			default:    return false;
		}
	}
	return result;
}
