#include <unistd.h>
#include "UnboundedQueue.h"
#include "iostream"

UnboundedQueue::UnboundedQueue() {
    sem_init(&this->full, 0, 0);
    sem_init(&this->mutex, 0, 1);
    this->queue = std::queue<std::string>();
}


void UnboundedQueue::insert(const std::string &str) {
    sem_wait(&this->mutex);
    this->queue.push(str);
    sem_post(&this->mutex);
    sem_post(&this->full);
}

std::string UnboundedQueue ::remove() {
    std::string str;
    sem_wait(&this->full);
    sem_wait(&this->mutex);
    str = this->queue.front();
    this->queue.pop();
    sem_post(&this->mutex);
    return str;
}