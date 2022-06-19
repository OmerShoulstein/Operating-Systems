#include "BoundedQueue.h"
#include "iostream"

void BoundedQueue::insert(const std::string &str) {
    empty.down();
    mutex.down();
    this->queue.push(str);
    mutex.up();
    full.up();
}

std::string BoundedQueue::remove() {
    std::string str;
    full.down();
    mutex.down();
    str = this->queue.front();
    this->queue.pop();
    mutex.up();
    empty.up();
    return str;
}

BoundedQueue::BoundedQueue(int size) : empty(size), mutex(1), full(0) {
    this->queue = std::queue<std::string>();
}

