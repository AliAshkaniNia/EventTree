#ifndef EVENT_PRODUCER_H
#define EVENT_PRODUCER_H

#include <memory>

#include "eventHub/IEventHub.h"

namespace eventTree::eventProducers {

/**
 * @class EventProducer
 * @brief Base class for event-producing entities.
 *
 * EventProducer provides a common interface for classes that generate events.
 * It holds a reference to an IEventHub for emitting events.
 */
class EventProducer {
   public:
    /** @brief Type alias for the shared pointer to IEventHub. */
    using EventEmitterPointer = std::shared_ptr<eventHubs::IEventHub>;

    /**
     * @brief Constructor that takes an EventEmitterPointer.
     * @param eventEmitter Shared pointer to an IEventHub instance.
     */
    explicit EventProducer(EventEmitterPointer eventEmitter)  // NOLINT
        : eventEmitter(eventEmitter) {}                       // NOLINT

    /**
     * @brief Getter for the event emitter.
     * @return Shared pointer to the IEventHub instance.
     */
    EventEmitterPointer getEventEmitter() { return eventEmitter; }

    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of derived classes.
     */
    virtual ~EventProducer() = default;
    // Special constructors to comply with "rule of 5"
    EventProducer(const EventProducer&) = default;
    EventProducer& operator=(const EventProducer&) = default;
    EventProducer(EventProducer&&) noexcept = default;
    EventProducer& operator=(EventProducer&&) noexcept = default;

   private:
    EventEmitterPointer
        eventEmitter; /**< Shared pointer to the IEventHub instance */

    /**
     * @brief Pure virtual function to produce events.
     *
     * This function should be implemented by derived classes to define
     * how events are produced and emitted.
     */
    virtual void produceEvents() = 0;
};

}  // namespace eventTree::eventProducers

#endif  // EVENT_PRODUCER_H