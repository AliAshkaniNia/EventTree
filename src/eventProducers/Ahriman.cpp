#include "eventProducers/Ahriman.h"

#include <chrono>
#include <memory>
#include <thread>

#include "events/Chaos.h"
#include "events/Event.h"
#include "events/Flood.h"

void eventTree::eventProducers::Ahriman::produceEvents() {
    auto emitter = getEventEmitter();
    if (emitter == nullptr) {
        return;
    }
    for (int i = 0; i < 4; ++i) {  // NOLINT

        emitter->emitEvent(events::EventType::Flood,
                           std::make_shared<events::Flood>("Iran"));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // NOLINT

        emitter->emitEvent(events::EventType::Chaos,
                           std::make_shared<events::Chaos>("Iraq"));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // NOLINT
    }
}
