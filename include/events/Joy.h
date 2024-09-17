#ifndef JOY_H
#define JOY_H

#include <string>
#include <utility>

#include "Event.h"

namespace eventTree::events {

/**
 * @class Joy
 * @brief Represents a Joy event in the event system.
 *
 * This class inherits from the Event base class and adds specific
 * functionality for Joy events.
 */
class Joy : public Event {
   public:
    /**
     * @brief Constructs a Joy event.
     * @param target_land The land targeted by the joy.
     */
    explicit Joy(std::string target_land)
        : Event(EventType::Joy), target_land(std::move(target_land)) {}

    /**
     * @brief Gets the target land of the joy.
     * @return The name of the land targeted by the joy.
     */
    std::string get_target_land() { return target_land; }

   private:
    std::string target_land; /**< The land targeted by the joy */
};

}  // namespace eventTree::events

#endif  // JOY_H