#include "BoundedQueue.h"
#include "iostream"

void BoundedQueue::insert(const std::string &str) {
    sem_wait(&empty);
    sem_wait(&mutex);
    this->queue.push(str);
    sem_post(&mutex);
    sem_post(&full);
}

std::string BoundedQueue::remove() {
    std::string str;
    sem_wait(&full);
    sem_wait(&mutex);
    str = this->queue.front();
    this->queue.pop();
    sem_post(&mutex);
    sem_post(&empty);
    return str;
}

BoundedQueue::BoundedQueue(int size) {
    sem_init(&this->empty, 0, size);
    sem_init(&this->full, 0, 0);
    sem_init(&this->mutex, 0, 1);
    this->queue = std::queue<std::string>();
}

