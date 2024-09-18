#include "eventHub/SpecialHub.h"

#include <chrono>
#include <functional>
#include <thread>

#include "events/Event.h"

eventTree::eventHubs::SpecialHub::SpecialHub()
    : dispatch_thread(&eventTree::eventHubs::SpecialHub::dispatchEvents, this) {
}

eventTree::eventHubs::SpecialHub::~SpecialHub() {
    running = false;
    if (dispatch_thread.joinable()) {
        dispatch_thread.join();
    }
}

void eventTree::eventHubs::SpecialHub::dispatchEvents() {
    while (running) {
        queue.processOne();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // NOLINT
    }
}

void eventTree::eventHubs::SpecialHub::emitEvent(events::EventType type,
                                                 events::EventPtr event) {
    queue.enqueue(type, event);
}

void eventTree::eventHubs::SpecialHub::registerHandler(
    events::EventType type, std::function<void(events::EventPtr)> func) {
    queue.appendListener(type, func);
}
