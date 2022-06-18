#include "CountingSemaphore.h"

CountingSemaphore::CountingSemaphore(int startValue) : m_value(startValue) {
    delay_mutex.lock();
}

void CountingSemaphore::down() {
    lock_mutex.lock();
    m_value--;
    if (m_value < 0) {
        lock_mutex.unlock();
        delay_mutex.lock();
    }

    lock_mutex.unlock();
}

void CountingSemaphore::up() {
    lock_mutex.lock();
    m_value++;
    if (m_value <= 0) {
        delay_mutex.unlock();
    } else {
        lock_mutex.unlock();
    }
}