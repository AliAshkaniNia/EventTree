#include <benchmark/benchmark.h>
#include <eventpp/eventqueue.h>

#include <chrono>
#include <cmath>
#include <limits>
#include <numeric>
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
    std::chrono::high_resolution_clock::time_point timestamp;
};

using HandlerType = std::function<void(const Event&)>;

using NaiveSpecialQueue =
    SpecialEventQueue<EventType, HandlerType, NaiveQueue<Event>>;

using ConcurrentSpecialQueue =
    SpecialEventQueue<EventType, HandlerType, MoodycamelQueue<Event>>;

using EventppQueue = eventpp::EventQueue<EventType, void(const Event&)>;

struct ResponseTimeStats {
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::lowest();
    double sum = 0;
    double sum_sq = 0;
    size_t count = 0;

    void update(double value) {
        min = std::min(min, value);
        max = std::max(max, value);
        sum += value;
        sum_sq += value * value;
        ++count;
    }

    double avg() const { return count > 0 ? sum / count : 0; }

    double std_dev() const {
        if (count < 2) return 0;
        double mean = avg();
        return std::sqrt((sum_sq / count) - (mean * mean));
    }
};

template <typename QueueType>
static void BM_ResponseTime(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        QueueType queue;
        ResponseTimeStats stats;

        // Add handlers
        for (int i = 0; i < 3; ++i) {
            EventType type = static_cast<EventType>(i);
            queue.appendListener(type, [&stats](const Event& e) {
                auto now = std::chrono::high_resolution_clock::now();
                double response_time =
                    std::chrono::duration<double, std::micro>(now - e.timestamp)
                        .count();
                stats.update(response_time);
            });
        }

        // Enqueue events
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 2);
        for (int i = 0; i < state.range(0); ++i) {
            EventType type = static_cast<EventType>(dis(gen));
            Event event{type, i, std::chrono::high_resolution_clock::now()};
            queue.enqueue(type, event);
        }
        state.ResumeTiming();

        for (int i = 0; i < state.range(0); ++i) {
            queue.processOne();
        }

        state.PauseTiming();
        state.counters["Min_Response_Time_us"] = benchmark::Counter(stats.min);
        state.counters["Max_Response_Time_us"] = benchmark::Counter(stats.max);
        state.counters["Avg_Response_Time_us"] =
            benchmark::Counter(stats.avg());
        state.counters["StdDev_Response_Time_us"] =
            benchmark::Counter(stats.std_dev());
        state.counters["Processed_Events"] = benchmark::Counter(stats.count);
        state.ResumeTiming();
    }
}

BENCHMARK_TEMPLATE(BM_ResponseTime, NaiveSpecialQueue)->Range(8, 8 << 10);
BENCHMARK_TEMPLATE(BM_ResponseTime, ConcurrentSpecialQueue)->Range(8, 8 << 10);

static void BM_ResponseTime_Eventpp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        EventppQueue queue;
        ResponseTimeStats stats;

        for (int i = 0; i < 3; ++i) {
            EventType type = static_cast<EventType>(i);
            queue.appendListener(type, [&stats](const Event& e) {
                auto now = std::chrono::high_resolution_clock::now();
                double response_time =
                    std::chrono::duration<double, std::micro>(now - e.timestamp)
                        .count();
                stats.update(response_time);
            });
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 2);
        for (int i = 0; i < state.range(0); ++i) {
            EventType type = static_cast<EventType>(dis(gen));
            Event event{type, i, std::chrono::high_resolution_clock::now()};
            queue.enqueue(type, event);
        }
        state.ResumeTiming();

        for (int i = 0; i < state.range(0); ++i) {
            queue.processOne();
        }

        state.PauseTiming();
        state.counters["Min_Response_Time_us"] = benchmark::Counter(stats.min);
        state.counters["Max_Response_Time_us"] = benchmark::Counter(stats.max);
        state.counters["Avg_Response_Time_us"] =
            benchmark::Counter(stats.avg());
        state.counters["StdDev_Response_Time_us"] =
            benchmark::Counter(stats.std_dev());
        state.counters["Processed_Events"] = benchmark::Counter(stats.count);
        state.ResumeTiming();
    }
}

BENCHMARK(BM_ResponseTime_Eventpp)->Range(8, 8 << 10);

BENCHMARK_MAIN();