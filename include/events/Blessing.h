#ifndef BLESSING_H
#define BLESSING_H

#include <string>
#include <utility>

#include "Event.h"

namespace eventTree::events {

/**
 * @class Blessing
 * @brief Represents a Blessing event in the event system.
 *
 * This class inherits from the Event base class and adds specific
 * functionality for Blessing events.
 */
class Blessing : public Event {
   public:
    /**
     * @brief Constructs a Blessing event.
     * @param target_land The land targeted by the blessing.
     */
    explicit Blessing(std::string target_land)
        : Event(EventType::Blessing), target_land(std::move(target_land)) {}

    /**
     * @brief Gets the target land of the blessing.
     * @return The name of the land targeted by the blessing.
     */
    std::string get_target_land() { return target_land; }

   private:
    std::string target_land; /**< The land targeted by the blessing */
};

}  // namespace eventTree::events

#endif  // BLESSING_H