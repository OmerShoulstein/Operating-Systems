#include <unistd.h>
#include "UnboundedQueue.h"
#include "iostream"

UnboundedQueue::UnboundedQueue() : full(0), mutex(1) {
    this->queue = std::queue<std::string>();
}


void UnboundedQueue::insert(const std::string &str) {
    mutex.down();
    this->queue.push(str);
    mutex.up();
    full.up();
}

std::string UnboundedQueue::remove() {
    std::string str;
    full.down();
    mutex.down();
    str = this->queue.front();
    this->queue.pop();
    mutex.up();
    return str;
}