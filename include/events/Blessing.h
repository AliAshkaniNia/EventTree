#ifndef BLESSING_H
#define BLESSING_H

#include <string>
#include <utility>

#include "Event.h"

namespace eventTree::events {
class Blessing : public Event {
   public:
    explicit Blessing(std::string target_land)
        : Event(EventType::Blessing), target_land(std::move(target_land)) {}
    std::string get_target_land() { return target_land; }

   private:
    std::string target_land;
};

}  // namespace eventTree::events

#endif  // BLESSING_H