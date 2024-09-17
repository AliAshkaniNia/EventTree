#ifndef ANAHITA_H
#define ANAHITA_H

#include "EventProducer.h"

namespace eventTree::eventProducers {

/**
 * @class Anahita
 * @brief Event producer class inspired by the ancient Persian goddess Anahita.
 *
 * Anahita, in ancient Persian mythology, was the goddess of water, fertility,
 * and wisdom. She was associated with rivers, lakes, and the planet Venus. As a
 * powerful deity, Anahita was believed to bring life and abundance.
 *
 * In the context of this event system, the Anahita class serves as an event
 * producer, potentially generating events related to natural phenomena, cycles,
 * or wisdom-based occurrences. This class extends the EventProducer base class
 * to implement specific event production logic.
 */
class Anahita : public EventProducer {
   public:
    /**
     * @brief Inherit constructors from EventProducer.
     *
     * This using declaration allows Anahita to use the constructor(s) from
     * EventProducer.
     */
    using EventProducer::EventProducer;

    /**
     * @brief Produces events specific to Anahita.
     *
     * This function overrides the pure virtual function from EventProducer.
     * It should implement the logic for generating and emitting events.
     */
    void produceEvents() override;
};

}  // namespace eventTree::eventProducers

#endif  // ANAHITA_H