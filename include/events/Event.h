#ifndef EVENTS_H
#define EVENTS_H

#include <cstdint>
#include <memory>

namespace eventTree::events {

/**
 * @enum EventType
 * @brief Enumerates the types of events in the system.
 */
enum class EventType : std::uint8_t { Blessing, Joy, Chaos, Flood };

/**
 * @class Event
 * @brief Base class for all events in the system.
 */
class Event {
   public:
    /**
     * @brief Constructs an Event with a specific type.
     * @param type The type of the event.
     */
    explicit Event(EventType type) : type(type) {}

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~Event() = default;

    /**
     * @brief Gets the type of the event.
     * @return The EventType of this event.
     */
    [[nodiscard]] EventType getType() const { return type; }

    // Special constructors to comply with "rule of 5"
    Event(const Event&) = default;
    Event& operator=(const Event&) = default;
    Event(Event&&) noexcept = default;
    Event& operator=(Event&&) noexcept = default;

   private:
    EventType type; /**< The type of this event */
};

/**
 * @typedef EventPtr
 * @brief Shared pointer to an Event object.
 */
using EventPtr = std::shared_ptr<Event>;

/**
 * @struct EventPolicy
 * @brief Defines the policy for handling events in the event system.
 */
struct EventPolicy {
    /**
     * @brief Extracts the event type from an EventPtr.
     * @param event Shared pointer to the event.
     * @return The EventType of the given event.
     */
    static EventType getEvent(const EventPtr& event) {
        return event->getType();
    }
};

}  // namespace eventTree::events

#endif  // EVENTS_H