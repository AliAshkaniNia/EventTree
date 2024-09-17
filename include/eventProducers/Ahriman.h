#ifndef AHRIMAN_H
#define AHRIMAN_H

#include "EventProducer.h"

namespace eventTree::eventProducers {
class Ahriman : public EventProducer {
   public:
    using EventProducer::EventProducer;
    void produceEvents() override;
};
}  // namespace eventTree::eventProducers

#endif  // AHRIMAN_H