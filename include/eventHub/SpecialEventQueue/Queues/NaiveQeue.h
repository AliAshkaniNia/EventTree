#ifndef NAIVE_QUEUE_H
#define NAIVE_QUEUE_H

#include <concepts>
#include <mutex>
#include <queue>

namespace eventTree::queues {
/**
 * @class NaiveQueue
 * @brief A thread-safe queue implementation.
 *
 * @tparam T The type of elements stored in the queue.
 *
 * This class provides a simple thread-safe queue implementation using
 * a mutex for synchronization. It allows pushing elements into the queue
 * and popping elements from the queue in a thread-safe manner.
 */
template <typename T>
class NaiveQueue {
   private:
    std::queue<T> queue;
    std::mutex mutex;

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
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::forward<U>(item));
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
    bool pop(T& item) {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty()) {
            return false;
        }
        item = std::move(queue.front());
        queue.pop();
        return true;
    }
};

}  // namespace eventTree::queues
#endif  // NAIVE_QUEUE_H