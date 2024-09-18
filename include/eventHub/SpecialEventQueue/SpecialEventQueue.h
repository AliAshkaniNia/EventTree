/**
 * @file SpecialEventQueue.h
 * @brief A minimal, thread-safe event queue library in C++.
 *
 * @details This library provides a flexible and efficient event queue system
 * with the following features:
 * - Template-based design for handling various event and handler types
 * - Thread-safe operations using TBB concurrent containers
 * - Support for multiple event types and handlers
 * - Fair event processing to prevent starvation of less frequent event types
 * - Concept-based constraints to ensure type safety and correct usage
 *
 * The main class, SpecialEventQueue, allows users to enqueue events, process
 * them, and manage event handlers. It uses a round-robin approach in the
 * processOne() function to ensure fairness in event processing.
 */

#ifndef SPECIAL_EVENT_QUEUE_H
#define SPECIAL_EVENT_QUEUE_H

#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_vector.h>

#include <atomic>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <tuple>
#include <utility>

namespace eventTree::eventHubs {

/**
 * @brief Concept to check if a type is default constructible.
 * @tparam T The type to check.
 */
template <typename T>
concept DefaultConstructible = requires { T{}; };

/**
 * @brief Concept to check if a type has a value_type.
 * @tparam T The type to check.
 */
template <typename T>
concept HasValueTypeConcept = requires { typename T::value_type; };

/**
 * @brief Concept to check if a type can be hashed.
 * @tparam T The type to check.
 */
template <typename T>
concept HashableConcept = requires(T type) {
    { std::hash<T>{}(type) } -> std::convertible_to<std::size_t>;
};

/**
 * @brief Concept defining the requirements for a Queue type.
 * @tparam Q The queue type to check.
 */
template <typename Q>
concept QueueConcept =
    HasValueTypeConcept<Q> && requires(Q queue, typename Q::value_type& lvalue,
                                       const typename Q::value_type& clvalue,
                                       typename Q::value_type&& rvalue) {
        { queue.push(clvalue) } -> std::same_as<void>;
        { queue.push(std::move(rvalue)) } -> std::same_as<void>;
        { queue.pop(lvalue) } -> std::same_as<bool>;
    };

/**
 * @brief Concept defining the requirements for a Handler type.
 * @tparam H The handler type to check.
 * @tparam Q The queue type associated with the handler.
 */
template <typename H, typename Q>
concept HandlerConcept = std::invocable<H, const typename Q::value_type&>;

/**
 * @class SpecialEventQueue
 * @brief A thread-safe event queue system supporting multiple event types and
 * handlers.
 *
 * @tparam EventType The type used to identify different events.
 * @tparam HandlerType The type of the event handlers.
 * @tparam QueueType The type of queue used to store events.
 */
template <typename EventType, typename HandlerType, typename QueueType>
    requires HashableConcept<EventType> && DefaultConstructible<QueueType> &&
             QueueConcept<QueueType> && HandlerConcept<HandlerType, QueueType>
class SpecialEventQueue {
   private:
    tbb::concurrent_unordered_map<EventType, QueueType> queues;  // NOLINT
    tbb::concurrent_unordered_multimap<EventType, HandlerType>   // NOLINT
        handlers;                                                // NOLINT
    tbb::concurrent_vector<EventType> eventTypes;                // NOLINT
    std::atomic<std::uint64_t> currentIndex{0};

    /**
     * @brief Get or create a queue for a given event type.
     * @param type The event type.
     * @return A reference to the queue for the given event type.
     */
    QueueType& getQueue(const EventType& type) {
        auto [iter, inserted] =
            queues.emplace(std::piecewise_construct,
                           std::forward_as_tuple(type), std::tuple<>());
        if (inserted) {
            eventTypes.push_back(type);
        }
        return iter->second;
    }

   public:
    /**
     * @brief Enqueue an event of a specific type.
     * @tparam T The type of the event to enqueue.
     * @param type The event type.
     * @param event The event to enqueue.
     */
    template <typename T>
    void enqueue(const EventType& type, T&& event) {
        getQueue(type).push(std::forward<T>(event));
    }

    /**
     * @brief Process one event from the queues.
     *
     * This function implements a fair processing strategy by using a
     * round-robin approach. It cycles through all event types, ensuring that a
     * burst of events of one type does not cause events of other types to
     * starve.
     *
     * @warning Assumes 'queues' and 'handlers' never delete an entry.
     *
     * @note This function may not always process an event in a single call.
     * It uses a round-robin method to select the target queue, and if that
     * queue is empty, it returns without checking subsequent queues. This
     * design choice was made to maintain code simplicity, as this method is
     * typically called in a loop within a thread. Occasional non-processing
     * iterations do not significantly impact overall performance and are
     * expected as part of normal operation.
     */
    void processOne() {
        auto eventTypeCount = eventTypes.size();
        if (eventTypeCount == 0) {
            return;
        }

        auto index = currentIndex.fetch_add(1, std::memory_order_relaxed) %
                     eventTypeCount;

        auto eventType = eventTypes[index];

        auto queueIter = queues.find(eventType);
        if (queueIter != queues.end()) {
            typename QueueType::value_type event;
            if (queueIter->second.pop(event)) {
                auto range = handlers.equal_range(eventType);
                for (auto handlerIt = range.first; handlerIt != range.second;
                     ++handlerIt) {
                    std::invoke(handlerIt->second, event);
                }
            }
        }
    }

    /**
     * @brief Add a new event handler for a specific event type.
     * @tparam H The type of the handler function.
     * @param type The event type to associate with the handler.
     * @param handler The handler function to add.
     */
    template <typename H>
    void appendListener(const EventType& type, H&& handler) {
        handlers.emplace(type, std::forward<H>(handler));
    }
};
}  // namespace eventTree::eventHubs
#endif