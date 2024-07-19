#pragma once

#include "../../LibCustomGui.hpp"
#include <vector>

class LogType {
  private:
    int id;
    const char *m_log_type;
    const char *m_log_type_text;
    ImVec4 m_log_color;

  public:
    LogType() : LogType(0, "EMPTY", "", ImVec4(1, 1, 1, 1)) {}

    LogType(int id, const char *log_type, const char *log_type_text, ImVec4 color)
        : id(id), m_log_type(log_type), m_log_type_text(log_type_text), m_log_color(color) {}

    friend bool operator<(LogType t1, LogType t2) { return t1.id < t2.id; }

    inline const char *getLogType() { return this->m_log_type; }

    inline const char *getLogTypeText() { return this->m_log_type_text; }

    inline ImVec4 getColor() { return this->m_log_color; }

    ~LogType() = default;
};

// namespace for all logs (no custom logs)
namespace logs {
inline LogType GENERAL_EVENT = LogType(1, "EVENT", "Event", ImVec4(0, 0.8, 0.1, 1));
inline LogType MISSING_IMPLEMENTATION = LogType(2, "MISSING", "Implementation", ImVec4(1, 0.5, 0, 1));
inline LogType MISSING_CASTER = LogType(3, "CASTER", "Caster", ImVec4(1, 1, 0, 1));
inline LogType SELECT_WARNING = LogType(4, "SELECT", "Select W", ImVec4(1, 1, 0, 1));
inline LogType ERROR = LogType(5, "ERROR", "Error", ImVec4(1, 0, 0, 1));
inline LogType EMPTY = LogType(0, "", "Empty", ImVec4(1, 1, 1, 1));

inline std::vector<LogType> log_types = {GENERAL_EVENT, MISSING_IMPLEMENTATION, MISSING_CASTER, SELECT_WARNING, ERROR, EMPTY};

} // namespace logs

class Log {
  private:
    LogType m_type;
    float m_time;
    const char *m_text;

  public:
    Log() = delete;

    Log(LogType type = LogType(), float time = 0.f, const char *text = "") : m_type(type), m_time(time), m_text(text) {}

    Log(int id, const char *log_type, const char *log_type_text, ImVec4 color, float time, const char *text)
        : Log(LogType(id, log_type, log_type_text, color), time, text) {}

    inline const char *getLogType() { return this->m_type.getLogType(); }

    inline LogType getLogTypeObject() { return this->m_type; }

    inline const char *getLogTypeText() { return this->m_type.getLogTypeText(); }

    inline ImVec4 getLogTypeColor() { return this->m_type.getColor(); }

    inline float getLogTime() { return this->m_time; }

    inline const char *getLogText() { return this->m_text; }

    ~Log() = default;
};
