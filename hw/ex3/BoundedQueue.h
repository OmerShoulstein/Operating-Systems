#ifndef EX3_BOUNDEDQUEUE_H
#define EX3_BOUNDEDQUEUE_H

#include <string>
#include <queue>
#include <semaphore.h>

class BoundedQueue{
private:
    sem_t full{};
    sem_t empty{};
    sem_t mutex{};
    std:: queue<std::string> queue;
public:
    explicit BoundedQueue(int size);

    void insert(const std::string &);

    std::string remove();

};


#endif //EX3_BOUNDEDQUEUE_H
