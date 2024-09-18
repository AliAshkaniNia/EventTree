#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <random>
#include <thread>

#include "eventHub/SpecialEventQueue/Queues/NaiveQeue.h"
#include "eventHub/SpecialEventQueue/SpecialEventQueue.h"

using namespace eventTree::eventHubs;
using namespace eventTree::queues;

// Mock handler for testing
template <typename T>
class MockHandler {
   public:
    MOCK_METHOD(void, Handle, (const T&), ());
};

// Event types for testing
enum class TestEventType { TypeA, TypeB, TypeC };

// Test event structure
struct TestEvent {
    using value_type = int;

    int value;
    explicit TestEvent(int v) : value(v) {}
    TestEvent() {}
};

class SpecialEventQueueTest : public ::testing::Test {
   protected:
    using TestQueue =
        SpecialEventQueue<TestEventType, std::function<void(const TestEvent&)>,
                          NaiveQueue<TestEvent>>;
    TestQueue queue;
};

// Basic functionality tests
TEST_F(SpecialEventQueueTest, EnqueueAndProcessSingleEvent) {
    MockHandler<TestEvent> mockHandler;
    EXPECT_CALL(mockHandler, Handle(::testing::_)).Times(1);

    queue.appendListener(TestEventType::TypeA, [&](const TestEvent& event) {
        mockHandler.Handle(event);
    });
    queue.enqueue(TestEventType::TypeA, TestEvent(42));
    queue.processOne();
}

TEST_F(SpecialEventQueueTest, ProcessEmptyQueue) {
    queue.processOne();  // Should not crash or throw
}

TEST_F(SpecialEventQueueTest, MultipleHandlersForSameEventType) {
    MockHandler<TestEvent> mockHandler1, mockHandler2;
    EXPECT_CALL(mockHandler1, Handle(::testing::_)).Times(1);
    EXPECT_CALL(mockHandler2, Handle(::testing::_)).Times(1);

    queue.appendListener(TestEventType::TypeA, [&](const TestEvent& event) {
        mockHandler1.Handle(event);
    });
    queue.appendListener(TestEventType::TypeA, [&](const TestEvent& event) {
        mockHandler2.Handle(event);
    });
    queue.enqueue(TestEventType::TypeA, TestEvent(42));
    queue.processOne();
}

// Thread safety test
TEST_F(SpecialEventQueueTest, ConcurrentEnqueueAndProcess) {
    const int NUM_THREADS = 10;
    const int EVENTS_PER_THREAD = 1000;

    std::atomic<int> processedEvents(0);
    std::atomic<int> producedEvents(0);

    auto enqueueFunc = [&](TestEventType type) {
        for (int i = 0; i < EVENTS_PER_THREAD; ++i) {
            queue.enqueue(type, TestEvent(i));
            producedEvents++;
        }
    };

    auto processFunc = [&]() {
        for (int i = 0; i < EVENTS_PER_THREAD * 2; ++i) {
            queue.processOne();
        }
    };

    queue.appendListener(TestEventType::TypeA,
                         [&](const TestEvent&) { processedEvents++; });
    queue.appendListener(TestEventType::TypeB,
                         [&](const TestEvent&) { processedEvents++; });

    std::vector<std::thread> threads;
    threads.emplace_back(enqueueFunc, TestEventType::TypeA);
    threads.emplace_back(enqueueFunc, TestEventType::TypeB);

    // Wait until some events get generated
    std::this_thread::sleep_for(std::chrono::seconds(1));

    threads.emplace_back(processFunc);

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(processedEvents.load(), producedEvents.load());
}

// Fairness test
TEST_F(SpecialEventQueueTest, FairnessTest) {
    const int EVENTS_PER_TYPE = 1200;
    std::atomic<int> countA(0), countB(0), countC(0);

    queue.appendListener(TestEventType::TypeA,
                         [&](const TestEvent&) { countA++; });
    queue.appendListener(TestEventType::TypeB,
                         [&](const TestEvent&) { countB++; });
    queue.appendListener(TestEventType::TypeC,
                         [&](const TestEvent&) { countC++; });

    // Enqueue events in bursts
    for (int i = 0; i < EVENTS_PER_TYPE; ++i) {
        queue.enqueue(TestEventType::TypeA, TestEvent(i));
    }
    for (int i = 0; i < EVENTS_PER_TYPE; ++i) {
        queue.enqueue(TestEventType::TypeB, TestEvent(i));
    }
    for (int i = 0; i < EVENTS_PER_TYPE; ++i) {
        queue.enqueue(TestEventType::TypeC, TestEvent(i));
    }

    // Process 1/3 of events
    for (int i = 0; i < EVENTS_PER_TYPE; ++i) {
        queue.processOne();
    }

    // Check if all event types were processed fairly
    EXPECT_NEAR(countA.load(), EVENTS_PER_TYPE / 3, EVENTS_PER_TYPE / 3 * 0.1);
    EXPECT_NEAR(countB.load(), EVENTS_PER_TYPE / 3, EVENTS_PER_TYPE / 3 * 0.1);
    EXPECT_NEAR(countC.load(), EVENTS_PER_TYPE / 3, EVENTS_PER_TYPE / 3 * 0.1);
}

// Performance test
TEST_F(SpecialEventQueueTest, PerformanceTest) {
    const int NUM_EVENTS = 1000000;
    std::atomic<int> processedEvents(0);

    queue.appendListener(TestEventType::TypeA,
                         [&](const TestEvent&) { processedEvents++; });

    auto start = std::chrono::high_resolution_clock::now();

    // Enqueue events
    for (int i = 0; i < NUM_EVENTS; ++i) {
        queue.enqueue(TestEventType::TypeA, TestEvent(i));
    }

    // Process events
    for (int i = 0; i < NUM_EVENTS; ++i) {
        queue.processOne();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    EXPECT_EQ(processedEvents.load(), NUM_EVENTS);
    std::cout << "Time taken to enqueue and process " << NUM_EVENTS
              << " events: " << duration.count() << " ms" << std::endl;
}

// Stress test
TEST_F(SpecialEventQueueTest, StressTest) {
    const int NUM_THREADS = 20;
    const int OPERATIONS_PER_THREAD = 1'000'000;
    std::atomic<int> processedEvents(0);

    queue.appendListener(TestEventType::TypeA,
                         [&](const TestEvent&) { processedEvents++; });
    queue.appendListener(TestEventType::TypeB,
                         [&](const TestEvent&) { processedEvents++; });
    queue.appendListener(TestEventType::TypeC,
                         [&](const TestEvent&) { processedEvents++; });

    auto workerFunc = [&]() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> typeDist(0, 2);
        std::uniform_int_distribution<> operationDist(0, 1);

        for (int i = 0; i < OPERATIONS_PER_THREAD; ++i) {
            TestEventType type = static_cast<TestEventType>(typeDist(gen));
            if (operationDist(gen) == 0) {
                queue.enqueue(type, TestEvent(i));
            } else {
                queue.processOne();
            }
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(workerFunc);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Process any remaining events
    for (int i = 0; i < NUM_THREADS * OPERATIONS_PER_THREAD; ++i) {
        queue.processOne();
    }

    std::cout << "Total processed events in stress test: "
              << processedEvents.load() << std::endl;
}