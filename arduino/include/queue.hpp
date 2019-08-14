#ifndef __QUEUE__
#define __QUEUE__
#include "common.hpp"

#if IS_CONTROLLER
#include <cppQueue.h>
template<typename T>
class QueueImplementation {
private:
    Queue queue;
public:
    QueueImplementation(): queue(Queue(sizeof(T), 10, LIFO)) {}
    T pop() {
        T result;
        queue.pop(&result);
        return result;
    }
    void push(const T& value) {
        queue.push(&value);
    }
    bool empty() {
        return queue.isEmpty();
    }
};

#else

#include <queue>
template<typename T>
class QueueImplementation: public std::queue<T> {
public:
    T pop() {
        T result = std::queue<T>::front();
        std::queue<T>::pop();
        return result;
    }
};
#endif

#endif