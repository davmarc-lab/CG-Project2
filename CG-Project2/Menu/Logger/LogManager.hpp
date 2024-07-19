#pragma once

#include "Log.hpp"

#include <vector>

class LogManager {
  private:
    std::vector<Log> m_logs;

    int m_track = -1;

    inline static LogManager *pointer = nullptr;

  protected:
    LogManager() {}

  public:
    inline void resetTrack() { this->m_track = -1; }

    inline int getTrack() { return this->m_track; }

    inline void addLog(Log log) {
        this->m_track++;
        this->m_logs.push_back(log);
    }

    inline void addLog(LogType log, float time, const char *text) { this->addLog(Log(log, time, text)); }

    inline void addLog(const char *log_type, const char *log_type_text, ImVec4 color, float time, const char *text) {
        this->addLog(Log(log_type, log_type_text, color, time, text));
    }

    inline std::vector<Log> getLogs() { return this->m_logs; }

    inline void clear() { this->m_logs.clear(); }

    inline static LogManager *instance() {
        if (pointer == nullptr)
            pointer = new LogManager();
        return pointer;
    }
};
