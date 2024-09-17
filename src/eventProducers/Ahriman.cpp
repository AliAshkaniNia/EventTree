#include "eventProducers/Ahriman.h"

#include <chrono>
#include <memory>
#include <thread>

#include "events/Chaos.h"
#include "events/Flood.h"

void eventTree::eventProducers::Ahriman::produceEvents() {
    for (int i = 0; i < 4; ++i) {  // NOLINT

        getEventEmitter()->emitEvent(std::make_shared<events::Flood>("Iran"));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // NOLINT

        getEventEmitter()->emitEvent(std::make_shared<events::Chaos>("Iraq"));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // NOLINT
    }
}
