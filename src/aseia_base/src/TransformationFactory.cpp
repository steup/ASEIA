#include <TransformationFactory.h>

#include <vector>
#include <algorithm>

struct TransformationFactoryImpl : public TransformationFactory{
	std::vector<createFunc> mTrans;

	virtual TransID registerCreator(createFunc f) {
		auto it = std::find(mTrans.begin(), mTrans.end(), f);
		if(it == mTrans.end()) {
			mTrans.push_back(f);
			return mTrans.size();
		} else
			return (it-mTrans.begin())+1;
	}
	virtual TransPtr create(const EventType&out, const EventTypes& in, TransID trans) const {
		if(trans > 0 && trans <= mTrans.size())
			return mTrans[trans-1](out, in);
		else
			return TransPtr();
	}
} impl;

TransformationFactory& TransformationFactory::instance() {
	return impl;
}
