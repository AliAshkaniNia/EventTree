#include <benchmark/benchmark.h>
#include <eventpp/eventqueue.h>

#include <chrono>
#include <random>

#include "eventHub/SpecialEventQueue/Queues/MoodycamelQueue.h"
#include "eventHub/SpecialEventQueue/Queues/NaiveQeue.h"
#include "eventHub/SpecialEventQueue/SpecialEventQueue.h"

using namespace eventTree::eventHubs;
using namespace eventTree::queues;

enum class EventType { A, B, C };

struct Event {
    EventType type;
    int data;
};

using HandlerType = std::function<void(const Event&)>;

using NaiveSpecialQueue =
    SpecialEventQueue<EventType, HandlerType, NaiveQueue<Event>>;

using ConcurrentSpecialQueue =
    SpecialEventQueue<EventType, HandlerType, MoodycamelQueue<Event>>;

using EventppQueue = eventpp::EventQueue<EventType, void(const Event&)>;

template <typename QueueType>
static void BM_EmitEvents_Queue(benchmark::State& state) {
    QueueType queue;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);

    for (auto _ : state) {
        EventType type = static_cast<EventType>(dis(gen));
        Event event{type, 0};
        queue.enqueue(type, event);
    }
}

BENCHMARK_TEMPLATE(BM_EmitEvents_Queue, NaiveSpecialQueue)->Range(8, 8 << 10);
BENCHMARK_TEMPLATE(BM_EmitEvents_Queue, ConcurrentSpecialQueue)
    ->Range(8, 8 << 10);

static void BM_EmitEvents_Eventpp(benchmark::State& state) {
    EventppQueue queue;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);

    for (auto _ : state) {
        EventType type = static_cast<EventType>(dis(gen));
        Event event{type, 0};
        queue.enqueue(type, event);
    }
}
BENCHMARK(BM_EmitEvents_Eventpp)->Range(8, 8 << 10);
