#ifndef EVENT_PRODUCER_H
#define EVENT_PRODUCER_H

#include <memory>

#include "eventHub/IEventHub.h"

namespace eventTree::eventProducers {
class EventProducer {
   public:
    using EventEmitterPointer = std::shared_ptr<eventHubs::IEventHub>;

    explicit EventProducer(EventEmitterPointer eventEmitter)  // NOLINT
        : eventEmitter(eventEmitter) {}                       // NOLINT

    EventEmitterPointer getEventEmitter() { return eventEmitter; }

    virtual ~EventProducer() = default;
    // Special constructors to comply with "rule of 5"
    EventProducer(const EventProducer&) = default;
    EventProducer& operator=(const EventProducer&) = default;
    EventProducer(EventProducer&&) noexcept = default;
    EventProducer& operator=(EventProducer&&) noexcept = default;

   private:
    EventEmitterPointer eventEmitter;

    virtual void produceEvents() = 0;
};
}  // namespace eventTree::eventProducers

#endif  // EVENT_PRODUCER_H