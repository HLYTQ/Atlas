#pragma once

class Timestep {
public:
    Timestep(float time = 0.0f) : m_Time(time) {}

    float GetSeconds() const {
        return m_Time;
    }
    float GetMilliseconds() const {
        return m_Time * 1000.0f;
    }
    Timestep& operator=(const Timestep& other) {
        if (this != &other) {
            m_Time = other.m_Time;
        }
        return *this;
    }

private:
    float m_Time;
};
