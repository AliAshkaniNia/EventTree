#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "eventHub/EventppHub.h"
#include "eventHub/SpecialHub.h"
#include "eventProducers/Ahriman.h"
#include "eventProducers/Anahita.h"
#include "events/Blessing.h"
#include "events/Chaos.h"
#include "events/Event.h"
#include "events/Flood.h"
#include "events/Joy.h"

int main() {
    using eventTree::eventHubs::EventppHub;
    using eventTree::eventHubs::SpecialHub;

    using eventTree::events::EventPtr;

    using eventTree::eventProducers::Ahriman;
    using eventTree::eventProducers::Anahita;

    using eventTree::events::EventPtr;
    // EventType
    using eventTree::events::EventType;

    using eventTree::events::Blessing;
    using eventTree::events::Chaos;
    using eventTree::events::Flood;
    using eventTree::events::Joy;

    // auto hub = std::make_shared<EventppHub>();
    auto hub = std::make_shared<SpecialHub>();

    hub->registerHandler(EventType::Blessing, [](EventPtr event) {  // NOLINT
        auto blessing = std::static_pointer_cast<Blessing>(event);
        std::cout << "Blessing received for: " << blessing->get_target_land()
                  << "\n";
    });
    hub->registerHandler(EventType::Joy, [](EventPtr event) {  // NOLINT
        auto joy = std::static_pointer_cast<Joy>(event);
        std::cout << "Joy spread over: " << joy->get_target_land() << "\n";
    });

    hub->registerHandler(EventType::Chaos, [](EventPtr event) {  // NOLINT
        auto chaos = std::static_pointer_cast<Chaos>(event);
        std::cout << "Chaos unleashed in: " << chaos->get_target_land() << "\n";
    });

    hub->registerHandler(EventType::Flood, [](EventPtr event) {  // NOLINT
        auto flood = std::static_pointer_cast<Flood>(event);
        std::cout << "Flood affecting: " << flood->get_target_land() << "\n";
    });

    auto anahita = std::make_unique<Anahita>(hub);
    const std::jthread anahita_thread(&Anahita::produceEvents, anahita.get());

    auto ahriman = std::make_unique<Ahriman>(hub);
    const std::jthread ahriman_thread(&Ahriman::produceEvents, ahriman.get());
    std::this_thread::sleep_for(
        std::chrono::seconds(1));  // Allow time for all events to be processed

    return 0;
}