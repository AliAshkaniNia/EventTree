#ifndef SPECIAL_HUB_H
#define SPECIAL_HUB_H

#include <atomic>
#include <functional>
#include <thread>

#include "IEventHub.h"
#include "SpecialEventQueue/Queues/NaiveQeue.h"
#include "SpecialEventQueue/SpecialEventQueue.h"
#include "events/Event.h"

namespace eventTree::eventHubs {

/**
 * @class SpecialHub
 * @brief A concrete implementation of IEventHub using the custom-made
 * SpecialEventQueue library.
 *
 * SpecialHub provides an event handling mechanism using an event queue from the
 * SpecialEventQueue library. It allows for emitting events and registering
 * event handlers.
 */
class SpecialHub : public IEventHub {
   private:
    /** @brief Type alias for the event queue used internally. */
    using EventQueue = SpecialEventQueue<events::EventType,
                                         std::function<void(events::EventPtr)>,
                                         queues::NaiveQueue<events::EventPtr> >;

    EventQueue queue; /**< The event queue for storing and processing events. */
    std::thread dispatch_thread;     /**< Thread for dispatching events. */
    std::atomic<bool> running{true}; /**< Flag to control the dispatch loop. */

    /**
     * @brief Private method to dispatch events from the queue.
     *
     * This method runs in a separate thread and continuously processes events
     * from the queue.
     */
    void dispatchEvents();

   public:
    /**
     * @brief Default constructor.
     *
     * Initializes the event queue and starts the dispatch thread.
     */
    SpecialHub();

    /**
     * @brief Destructor.
     *
     * Stops the dispatch thread and cleans up resources.
     */
    ~SpecialHub() override;

    /**
     * @brief Emits an event to be processed by registered handlers.
     * @param type The type of event to handle.
     * @param event Shared pointer to the event to be emitted.
     */
    void emitEvent(events::EventType type, events::EventPtr event) override;

    /**
     * @brief Registers a handler function for a specific event type.
     * @param type The type of event to handle.
     * @param func The handler function to be called when the event occurs.
     */
    void registerHandler(events::EventType type,
                         std::function<void(events::EventPtr)> func) override;

    // Special constructors to comply with "rule of 5"
    SpecialHub(const SpecialHub&) = delete;
    SpecialHub& operator=(const SpecialHub&) = delete;
    SpecialHub(SpecialHub&&) noexcept = delete;
    SpecialHub& operator=(SpecialHub&&) noexcept = delete;
};

}  // namespace eventTree::eventHubs

#endif  // SPECIAL_HUB_H