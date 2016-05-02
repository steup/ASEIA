#include <MetaFilter.h>

/** \todo insert filter code */
bool MetaPredicate::operator()(const MetaEvent& e) const {
	return false;
}

/** \todo insert filter code */
bool MetaPredicate::operator()(const MetaEvent& e0, const MetaEvent& e1) const {
	return false;
}

/** \todo insert filter code */
bool MetaFilter::operator()(const std::vector<MetaEvent>& events) const {
	return false;
}
