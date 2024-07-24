#pragma once

class Breakpoint {
private:
    float m_time = 0;
public:
    Breakpoint() {}

    Breakpoint(float time) : m_time(time) {}

    inline float getTime() { return this->m_time; }

    inline void setTime(float time) { this->m_time = time; }

    ~Breakpoint() = default;
};
