#ifndef JOY_H
#define JOY_H

#include <string>
#include <utility>

#include "Event.h"

namespace eventTree::events {

class Joy : public Event {
   public:
    explicit Joy(std::string target_land)
        : Event(EventType::Joy), target_land(std::move(target_land)) {}
    std::string get_target_land() { return target_land; }

   private:
    std::string target_land;
};

}  // namespace eventTree::events

#endif  // JOY_H