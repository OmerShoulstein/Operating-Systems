#ifndef EX3_UNBOUNDEDQUEUE_H
#define EX3_UNBOUNDEDQUEUE_H

#include "CountingSemaphore.h"
#include <string>
#include <queue>

class UnboundedQueue {
private:
    CountingSemaphore full;
    CountingSemaphore mutex;
    std::queue<std::string> queue;
public:
    explicit UnboundedQueue();

    void insert(const std::string &);

    std::string remove();

};



#endif //EX3_UNBOUNDEDQUEUE_H
