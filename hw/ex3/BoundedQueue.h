#ifndef EX3_BOUNDEDQUEUE_H
#define EX3_BOUNDEDQUEUE_H

#include <string>
#include <queue>
#include "CountingSemaphore.h"

class BoundedQueue {
private:
    CountingSemaphore full;
    CountingSemaphore empty;
    CountingSemaphore mutex;
    std::queue<std::string> queue;
public:
    explicit BoundedQueue(int size);

    void insert(const std::string &);

    std::string remove();

};


#endif //EX3_BOUNDEDQUEUE_H
