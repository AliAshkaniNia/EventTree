#include "eventProducers/Anahita.h"

#include <chrono>
#include <memory>
#include <thread>

#include "events/Blessing.h"
#include "events/Event.h"
#include "events/Joy.h"

void eventTree::eventProducers::Anahita::produceEvents() {
    auto emitter = getEventEmitter();
    if (emitter == nullptr) {
        return;
    }
    for (int i = 0; i < 5; ++i) {  // NOLINT
        emitter->emitEvent(events::EventType::Blessing,
                           std::make_shared<events::Blessing>("Iran"));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // NOLINT

        emitter->emitEvent(events::EventType::Joy,
                           std::make_shared<events::Joy>("Iraq"));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // NOLINT
    }
}
