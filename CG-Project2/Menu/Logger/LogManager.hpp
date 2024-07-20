#pragma once

#include "Log.hpp"

#include <deque>


#define MAX_BUF_DIM 255

class LogManager {
  private:
    std::deque<Log> m_logs;

    bool m_autoclear = true;
    int m_track = -1;

    inline static LogManager *pointer = nullptr;

  protected:
    LogManager() {}

  public:
    inline void resetTrack() { this->m_track = -1; }

    inline int getTrack() { return this->m_track; }

    inline void addLog(Log log) {
        if (this->m_logs.size() >= MAX_BUF_DIM && this->m_autoclear) {
            this->m_logs.pop_front();
        } else {
            this->m_track++;
        }
        this->m_logs.push_back(log);
    }

    inline void addLog(LogType log, const char *text) { this->addLog(Log(log, text)); }

    inline void addLog(const char *log_type, const char *log_type_text, ImVec4 color, const char *text) {
        this->addLog(Log(log_type, log_type_text, color, text));
    }

    inline std::deque<Log> getLogs() { return this->m_logs; }

    inline void clear() { this->m_logs.clear(); }

    inline static LogManager *instance() {
        if (pointer == nullptr)
            pointer = new LogManager();
        return pointer;
    }

    inline bool getAutoclear() { return this->m_autoclear; }

    inline void setAutoclear(bool autoclear) { this->m_autoclear = autoclear; }

    inline static bool writeLogToFile(const char* path = "./resources/Log/") {

        return false;
    }
};
