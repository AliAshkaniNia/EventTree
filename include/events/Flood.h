#ifndef FLOOD_H
#define FLOOD_H

#include <string>
#include <utility>

#include "Event.h"

namespace eventTree::events {
class Flood : public Event {
   public:
    explicit Flood(std::string target_land)
        : Event(EventType::Flood), target_land(std::move(target_land)) {}
    std::string get_target_land() { return target_land; }

   private:
    std::string target_land;
};
}  // namespace eventTree::events

#endif  // FLOOD_H