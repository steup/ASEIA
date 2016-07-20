#pragma once

#include <memory>
#include <Transformation.h>
#include <MetaEvent.h>

class EventStorageImpl;

class EventStorage {
	private:
		EventStorageImpl* mImpl;
	public:
		enum class Policy {
			recent,
			minimumUncertainty,
			performance
		};
		EventStorage(std::size_t inputSize, Policy policy=Policy::recent);
		~EventStorage();
		void addEvent(const MetaEvent& e);
		void purge();
		MetaEvent executeTransform(Transformation& t) const;
};
