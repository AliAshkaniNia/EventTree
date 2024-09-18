#include "eventHub/EventppHub.h"

#include <chrono>
#include <functional>
#include <thread>

#include "events/Event.h"

eventTree::eventHubs::EventppHub::EventppHub()
    : dispatch_thread(&eventTree::eventHubs::EventppHub::dispatchEvents, this) {
}

eventTree::eventHubs::EventppHub::~EventppHub() {
    running = false;
    if (dispatch_thread.joinable()) {
        dispatch_thread.join();
    }
}

void eventTree::eventHubs::EventppHub::dispatchEvents() {
    while (running) {
        queue.processOne();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // NOLINT
    }
}

void eventTree::eventHubs::EventppHub::emitEvent(events::EventType type,
                                                 events::EventPtr event) {
    (void)type;  // To suppress warnings.
                 // eventpp uses the policy to detect the
                 // event type and does not need this variable

    queue.enqueue(event);
}

void eventTree::eventHubs::EventppHub::registerHandler(
    events::EventType type, std::function<void(events::EventPtr)> func) {
    queue.appendListener(type, func);
}
