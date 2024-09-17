#ifndef FLOOD_H
#define FLOOD_H

#include <string>
#include <utility>

#include "Event.h"

namespace eventTree::events {

/**
 * @class Flood
 * @brief Represents a Flood event in the event system.
 *
 * This class inherits from the Event base class and adds specific
 * functionality for Flood events.
 */
class Flood : public Event {
   public:
    /**
     * @brief Constructs a Flood event.
     * @param target_land The land targeted by the flood.
     */
    explicit Flood(std::string target_land)
        : Event(EventType::Flood), target_land(std::move(target_land)) {}

    /**
     * @brief Gets the target land of the flood.
     * @return The name of the land targeted by the flood.
     */
    std::string get_target_land() { return target_land; }

   private:
    std::string target_land; /**< The land targeted by the flood */
};

}  // namespace eventTree::events

#endif  // FLOOD_H