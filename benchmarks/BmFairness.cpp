/**
 * @file fairness_benchmark.cpp
 * @brief Benchmark for measuring fairness in event queue implementations.
 *
 * @details This benchmark compares the fairness of different event queue
 * implementations, including a NaiveSpecialQueue, ConcurrentSpecialQueue,
 * and the eventpp library's EventQueue. The benchmark focuses on how fairly
 * each implementation processes events of different types in a bursty scenario,
 * particularly in the early stages of processing.
 *
 * @section approach Approach
 *
 * The benchmark uses the following approach:
 * 1. Create a burst of 1000 events with a non-uniform distribution:
 *    - 60% EventType::A
 *    - 20% EventType::B
 *    - 20% EventType::C
 * 2. Shuffle these events to simulate a realistic, randomized input.
 * 3. Enqueue all events into the queue being tested.
 * 4. Process 1/4 of the total events (250) using the queue's processOne()
 * method.
 * 5. Count how many events of each type were processed in this subset.
 * 6. Calculate a fairness index based on these counts.
 *
 * @section fairness_index Fairness Index
 *
 * The fairness of each queue implementation is measured using Jain's fairness
 * index:
 *
 * F = (Σx)² / (n * Σ(x²))
 *
 * Where:
 * - x is the number of processed events for each type
 * - n is the number of event types
 * - Σ denotes the sum
 *
 * This index ranges from 1/n (worst case) to 1 (best case):
 * - 1 indicates perfect fairness (all event types processed equally)
 * - 1/n indicates maximum unfairness (only one event type processed)
 *
 */

#include <benchmark/benchmark.h>
#include <eventpp/eventqueue.h>

#include <array>
#include <chrono>
#include <random>
#include <vector>

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

double calculateFairnessIndex(const std::array<int, 3>& processedCounts) {
    double sum = 0;
    double sumSquared = 0;
    for (int count : processedCounts) {
        sum += count;
        sumSquared += count * count;
    }
    double n = processedCounts.size();
    return (sum * sum) / (n * sumSquared);
}

template <typename QueueType>
static void BM_Fairness_Queue(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        QueueType queue;
        std::array<int, 3> processedCounts = {0, 0, 0};

        // Add handlers
        for (int i = 0; i < 3; ++i) {
            EventType type = static_cast<EventType>(i);
            queue.appendListener(type, [&processedCounts, i](const Event& e) {
                processedCounts[i]++;
            });
        }

        // Create burst of 1000 events (60% A, 20% B, 20% C)
        std::vector<Event> events;
        events.reserve(1000);
        for (int i = 0; i < 1000; ++i) {
            EventType type = (i % 10 < 6)   ? EventType::A
                             : (i % 10 < 8) ? EventType::B
                                            : EventType::C;
            events.push_back(Event{type, i});
        }

        // Shuffle events to simulate real-world scenario
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(events.begin(), events.end(), g);

        for (const auto& event : events) {
            queue.enqueue(event.type, event);
        }

        state.ResumeTiming();

        // Process 1/4 of the events
        for (int i = 0; i < 250; ++i) {
            queue.processOne();
        }

        state.PauseTiming();
        // Calculate fairness index
        double fairnessIndex = calculateFairnessIndex(processedCounts);
        state.counters["Fairness_Index"] = benchmark::Counter(fairnessIndex);
        state.counters["Processed_A"] = benchmark::Counter(processedCounts[0]);
        state.counters["Processed_B"] = benchmark::Counter(processedCounts[1]);
        state.counters["Processed_C"] = benchmark::Counter(processedCounts[2]);
        state.ResumeTiming();
    }
}

BENCHMARK_TEMPLATE(BM_Fairness_Queue, NaiveSpecialQueue);
BENCHMARK_TEMPLATE(BM_Fairness_Queue, ConcurrentSpecialQueue);

static void BM_Fairness_Eventpp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        EventppQueue queue;
        std::array<int, 3> processedCounts = {0, 0, 0};

        for (int i = 0; i < 3; ++i) {
            EventType type = static_cast<EventType>(i);
            queue.appendListener(type, [&processedCounts, i](const Event& e) {
                processedCounts[i]++;
            });
        }

        // Create burst of 1000 events (60% A, 20% B, 20% C)
        std::vector<Event> events;
        events.reserve(1000);
        for (int i = 0; i < 1000; ++i) {
            EventType type = (i % 10 < 6)   ? EventType::A
                             : (i % 10 < 8) ? EventType::B
                                            : EventType::C;
            events.push_back(Event{type, i});
        }

        // Shuffle events to simulate real-world scenario
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(events.begin(), events.end(), g);

        for (const auto& event : events) {
            queue.enqueue(event.type, event);
        }

        state.ResumeTiming();

        // Process 1/4 of the events
        for (int i = 0; i < 250; ++i) {
            queue.processOne();
        }

        state.PauseTiming();
        // Calculate fairness index
        double fairnessIndex = calculateFairnessIndex(processedCounts);
        state.counters["Fairness_Index"] = benchmark::Counter(fairnessIndex);
        state.counters["Processed_A"] = benchmark::Counter(processedCounts[0]);
        state.counters["Processed_B"] = benchmark::Counter(processedCounts[1]);
        state.counters["Processed_C"] = benchmark::Counter(processedCounts[2]);
        state.ResumeTiming();
    }
}

BENCHMARK(BM_Fairness_Eventpp);
