#ifndef EX3_UNBOUNDEDQUEUE_H
#define EX3_UNBOUNDEDQUEUE_H

#include "semaphore.h"
#include <string>
#include <queue>

class UnboundedQueue {
private:
    sem_t full{};
    sem_t mutex{};
    std::queue<std::string> queue;
public:
    explicit UnboundedQueue();

    void insert(const std::string &);

    std::string remove();

};



#endif //EX3_UNBOUNDEDQUEUE_H
