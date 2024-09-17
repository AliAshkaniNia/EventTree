#ifndef EVENTPP_HUB_H
#define EVENTPP_HUB_H

#include <eventpp/eventqueue.h>

#include <atomic>
#include <functional>
#include <thread>

#include "IEventHub.h"
#include "events/Event.h"

namespace eventTree::eventHubs {
class EventppHub : public IEventHub {
   private:
    using EventQueue =
        eventpp::EventQueue<events::EventType, void(const events::EventPtr&),
                            events::EventPolicy>;
    EventQueue queue;
    std::thread dispatch_thread;
    std::atomic<bool> running{true};

    void dispatchEvents();

   public:
    EventppHub();
    ~EventppHub() override;

    void emitEvent(const events::EventPtr& event) override;
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