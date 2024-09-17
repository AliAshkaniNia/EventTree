#ifndef ANAHITA_H
#define ANAHITA_H

#include "EventProducer.h"

namespace eventTree::eventProducers {
class Anahita : public EventProducer {
   public:
    using EventProducer::EventProducer;
    void produceEvents() override;
};
}  // namespace eventTree::eventProducers

#endif  // ANAHITA_H