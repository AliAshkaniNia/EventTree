#ifndef CHAOS_H
#define CHAOS_H

#include <string>
#include <utility>

#include "Event.h"

namespace eventTree::events {

/**
 * @class Chaos
 * @brief Represents a Chaos event in the event system.
 *
 * This class inherits from the Event base class and adds specific
 * functionality for Chaos events.
 */
class Chaos : public Event {
   public:
    /**
     * @brief Constructs a Chaos event.
     * @param target_land The land targeted by the chaos.
     */
    explicit Chaos(std::string target_land)
        : Event(EventType::Chaos), target_land(std::move(target_land)) {}

    /**
     * @brief Gets the target land of the chaos.
     * @return The name of the land targeted by the chaos.
     */
    std::string get_target_land() { return target_land; }

   private:
    std::string target_land; /**< The land targeted by the chaos */
};

}  // namespace eventTree::events

#endif  // CHAOS_H