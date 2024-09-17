#ifndef AHRIMAN_H
#define AHRIMAN_H

#include "EventProducer.h"

namespace eventTree::eventProducers {

/**
 * @class Ahriman
 * @brief Event producer class inspired by the ancient Persian spirit Ahriman.
 *
 * Ahriman, also known as Angra Mainyu, is a prominent figure in Zoroastrianism,
 * an ancient Persian religion. He is the evil spirit of destruction and chaos,
 * the embodiment of darkness and the eternal adversary of Ahura Mazda, the wise
 * lord. Ahriman is associated with conflict, discord, and negative forces in
 * the universe.
 *
 * In the context of this event system, the Ahriman class serves as an event
 * producer, potentially generating events related to challenges, obstacles, or
 * disruptive occurrences. This class extends the EventProducer base class to
 * implement specific event production logic that may represent the chaotic or
 * challenging aspects of a system or scenario.
 */
class Ahriman : public EventProducer {
   public:
    /**
     * @brief Inherit constructors from EventProducer.
     *
     * This using declaration allows Ahriman to use the constructor(s) from
     * EventProducer.
     */
    using EventProducer::EventProducer;

    /**
     * @brief Produces events specific to Ahriman.
     *
     * This function overrides the pure virtual function from EventProducer.
     * It should implement the logic for generating and emitting events.
     */
    void produceEvents() override;
};

}  // namespace eventTree::eventProducers

#endif  // AHRIMAN_H