#ifndef EVENTPP_HUB_H
#define EVENTPP_HUB_H

#include <eventpp/eventqueue.h>

#include <atomic>
#include <functional>
#include <thread>

#include "IEventHub.h"
#include "events/Event.h"

namespace eventTree::eventHubs {

/**
 * @class EventppHub
 * @brief A concrete implementation of IEventHub using the eventpp library.
 *
 * EventppHub provides an event handling mechanism using an event queue from the
 * eventpp library. It allows for emitting events and registering event
 * handlers.
 */
class EventppHub : public IEventHub {
   private:
    /** @brief Type alias for the event queue used internally. */
    using EventQueue =
        eventpp::EventQueue<events::EventType, void(const events::EventPtr&),
                            events::EventPolicy>;

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
    EventppHub();

    /**
     * @brief Destructor.
     *
     * Stops the dispatch thread and cleans up resources.
     */
    ~EventppHub() override;

    /**
     * @brief Emits an event to be processed by registered handlers.
     * @param event Shared pointer to the event to be emitted.
     */
    void emitEvent(const events::EventPtr& event) override;

    /**
     * @brief Registers a handler function for a specific event type.
     * @param type The type of event to handle.
     * @param func The handler function to be called when the event occurs.
     */
    void registerHandler(
        events::EventType type,
        std::function<void(const events::EventPtr&)> func) override;

    // Special constructors to comply with "rule of 5"
    EventppHub(const EventppHub&) = delete;
    EventppHub& operator=(const EventppHub&) = delete;
    EventppHub(EventppHub&&) noexcept = delete;
    EventppHub& operator=(EventppHub&&) noexcept = delete;
};

}  // namespace eventTree::eventHubs

#endif  // EVENTPP_HUB_H