#ifndef MOODYCAMEL_QUEUE_H
#define MOODYCAMEL_QUEUE_H

#include <concurrentqueue.h>

#include <concepts>

namespace eventTree::queues {

/**
 * @class MoodycamelQueue
 * @brief A thread-safe queue adaptor using moodycamel::ConcurrentQueue.
 *
 * @tparam T The type of elements stored in the queue.
 *
 * This class provides a simple adaptor for moodycamel::ConcurrentQueue,
 * offering a thread-safe queue implementation with efficient concurrent
 * operations. It allows pushing elements into the queue and popping elements
 * from the queue in a thread-safe manner without explicit locking.
 */
template <typename T>
class MoodycamelQueue {
   private:
    moodycamel::ConcurrentQueue<T>
        queue;  ///< The underlying concurrent queue container.

   public:
    using value_type = T;  ///< The type of elements stored in the queue.

    /**
     * @brief Pushes an item into the queue.
     *
     * @tparam U The type of the item being pushed.
     * @param item The item to be pushed into the queue.
     *
     * This function is thread-safe and uses perfect forwarding to
     * efficiently move or copy the item into the queue.
     *
     * @note This function is enabled only if U is convertible to T.
     */
    template <typename U>
        requires std::convertible_to<U&&, T>
    void push(U&& item) {
        queue.enqueue(std::forward<U>(item));
    }

    /**
     * @brief Attempts to pop an item from the queue.
     *
     * @param[out] item The variable to store the popped item.
     * @return true if an item was successfully popped, false if the queue was
     * empty.
     *
     * This function is thread-safe. If the queue is empty, it returns false
     * and leaves the @p item parameter unchanged.
     */
    bool pop(T& item) { return queue.try_dequeue(item); }
};
}  // namespace eventTree::queues
#endif  // MOODYCAMEL_QUEUE_H