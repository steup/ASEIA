#pragma once

#include <Transformation.h>
#include <MetaEvent.h>
#include <memory>

class EventStorage {
	public:
    using Ptr = std::unique_ptr<EventStorage>;
		enum class Policy {
			recent,
			minimumUncertainty,
			performance
		};
    enum class Type {
      simple
    };
    static Ptr create(Type type, Policy policy);
		virtual ~EventStorage() {}
		virtual void addEvent(const MetaEvent& e) = 0;
		virtual void purge() = 0;
		virtual MetaEvent executeTransform(Transformer& t) const =0;
};
