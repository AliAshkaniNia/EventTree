#ifndef CHAOS_H
#define CHAOS_H

#include <string>
#include <utility>

#include "Event.h"

namespace eventTree::events {

class Chaos : public Event {
   public:
    explicit Chaos(std::string target_land)
        : Event(EventType::Chaos), target_land(std::move(target_land)) {}
    std::string get_target_land() { return target_land; }

   private:
    std::string target_land;
};

}  // namespace eventTree::events

#endif  // CHAOS_H