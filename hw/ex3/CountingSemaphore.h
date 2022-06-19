#ifndef COUNTING_SEMAPHORE_HPP
#define COUNTING_SEMAPHORE_HPP

#include <mutex>

using namespace std;

class CountingSemaphore {
private:
    int m_value;
    mutex lock_mutex;
    mutex delay_mutex;

public:
    CountingSemaphore(int startValue);

    void down();

    void up();
};

#endif