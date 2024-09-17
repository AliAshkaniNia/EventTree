#ifndef IEVENT_HUB_H
#define IEVENT_HUB_H

#include <functional>

#include "events/Event.h"

namespace eventTree::eventHubs {

/**
 * @class IEventHub
 * @brief Interface for event hub implementations.
 *
 * IEventHub defines the interface for event handling mechanisms.
 * It provides pure virtual functions for emitting events and registering event
 * handlers.
 */
class IEventHub {
   public:
    /**
     * @brief Virtual destructor.
     *
     * Ensures proper cleanup of derived classes.
     */
    virtual ~IEventHub() = default;

    /**
     * @brief Pure virtual function to emit an event.
     * @param event Shared pointer to the event to be emitted.
     */
    virtual void emitEvent(const events::EventPtr& event) = 0;

    /**
     * @brief Pure virtual function to register an event handler.
     * @param type The type of event to handle.
     * @param func The handler function to be called when the event occurs.
     */
    virtual void registerHandler(
        events::EventType type,
        std::function<void(const events::EventPtr&)> func) = 0;

    // Special constructors to comply with "rule of 5"

    IEventHub() = default;
    IEventHub(const IEventHub&) = default;
    IEventHub& operator=(const IEventHub&) = default;
    IEventHub(IEventHub&&) noexcept = default;
    IEventHub& operator=(IEventHub&&) noexcept = default;
};

}  // namespace eventTree::eventHubs

#endif  // IEVENT_HUB_H