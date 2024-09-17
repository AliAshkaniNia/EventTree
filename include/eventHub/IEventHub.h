#ifndef IEVENT_HUB_H
#define IEVENT_HUB_H

#include <functional>

#include "events/Event.h"

namespace eventTree::eventHubs {
class IEventHub {
   public:
    virtual ~IEventHub() = default;
    virtual void emitEvent(const events::EventPtr& event) = 0;
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