#include "CountingSemaphore.h"

CountingSemaphore::CountingSemaphore(int startValue) : m_value(startValue), m_startValue(startValue) {
    m_delay_s.lock();
}

void CountingSemaphore::down() {
    m_mutex_s.lock();
    m_value--;
    if (m_value < 0) {
        m_mutex_s.unlock();
        m_delay_s.lock();
    }

    m_mutex_s.unlock();
}

void CountingSemaphore::up() {
    m_mutex_s.lock();
    m_value++;
    if (m_value <= 0) {
        m_delay_s.unlock();
    } else {
        m_mutex_s.unlock();
    }
}

int CountingSemaphore::value() {
    return m_value;
}

void CountingSemaphore::setValue(int value) {
    m_value = value;
}