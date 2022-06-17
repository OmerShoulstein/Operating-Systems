#ifndef COUNTING_SEMAPHORE_HPP
#define COUNTING_SEMAPHORE_HPP

#include <mutex>

using namespace std;

class CountingSemaphore {
private:
    int m_value;
    const int m_startValue;
    mutex m_mutex_s;
    mutex m_delay_s;

public:
    CountingSemaphore(int startValue);

    void down();

    void up();

    int value();

    void setValue(int value);
};

#endif