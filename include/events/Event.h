#ifndef EVENTS_H
#define EVENTS_H

#include <cstdint>
#include <memory>

namespace eventTree::events {
enum class EventType : std::uint8_t { Blessing, Joy, Chaos, Flood };

class Event {
   public:
    explicit Event(EventType type) : type(type) {}
    virtual ~Event() = default;
    [[nodiscard]] EventType getType() const { return type; }

    // Special constructors to comply with "rule of 5"
    Event(const Event&) = default;
    Event& operator=(const Event&) = default;
    Event(Event&&) noexcept = default;
    Event& operator=(Event&&) noexcept = default;

   private:
    EventType type;
};

using EventPtr = std::shared_ptr<Event>;

struct EventPolicy {
    static EventType getEvent(const EventPtr& event) {
        return event->getType();
    }
};
};  // namespace eventTree::events

#endif  // EVENTS_H