#pragma once

#include "../../LibCustomGui.hpp"
#include <set>

static int auto_id = 0;

class LogType {
  private:
    int id;
    const char *m_log_type;
    const char *m_log_type_text;
    ImVec4 m_log_color;

  public:
    LogType() : LogType("EMPTY", "", ImVec4(1, 1, 1, 1)) {}

    LogType(const char *log_type, const char *log_type_text, ImVec4 color)
        : id(auto_id), m_log_type(log_type), m_log_type_text(log_type_text), m_log_color(color) {
        auto_id++;
    }

    inline static void incrementIndex() { auto_id++; }

    friend bool operator<(LogType t1, LogType t2) { return t1.id < t2.id; }

    inline const char *getLogType() { return this->m_log_type; }

    inline const char *getLogTypeText() { return this->m_log_type_text; }

    inline ImVec4 getColor() { return this->m_log_color; }

    ~LogType() = default;
};

// namespace for all logs (no custom logs)
namespace logs {
inline LogType EMPTY = LogType("", "Empty", ImVec4(1, 1, 1, 1));
inline LogType USER_MODE = LogType("MODE", "Mode", ImVec4(0.224, 0.902, 0.494, 1));
inline LogType GENERAL_EVENT = LogType("EVENT", "Event", ImVec4(0, 0.8, 0.1, 1));
inline LogType ADD_ENTITY = LogType("ADD", "Add", ImVec4(0, 0.8, 0.1, 1));
inline LogType REMOVE_ENTITY = LogType("REMOVE", "Remove", ImVec4(1, 1, 0, 1));
inline LogType MISSING_IMPLEMENTATION = LogType("MISSING", "Implementation", ImVec4(1, 0.5, 0, 1));
inline LogType MISSING_CASTER = LogType("CASTER", "Caster", ImVec4(1, 1, 0, 1));
inline LogType SELECT_WARNING = LogType("SELECT", "Select W", ImVec4(1, 1, 0, 1));
inline LogType ERROR = LogType("ERROR", "Error", ImVec4(1, 0, 0, 1));
inline LogType STATE = LogType("STATE", "Play", ImVec4(1, 0, 0, 1));
inline LogType INIT = LogType("INIT", "Init", ImVec4(1, 1, 0, 1));
inline LogType SILENCE = LogType("SILENCE", "Silence", ImVec4(1, 1, 1, 1));
inline std::set<LogType> log_types = {GENERAL_EVENT, USER_MODE, MISSING_IMPLEMENTATION, MISSING_CASTER, STATE, ADD_ENTITY, REMOVE_ENTITY, SELECT_WARNING, ERROR, SILENCE, EMPTY};

} // namespace logs

class Log {
  private:
    LogType m_type;
    float m_time;
    const char *m_text;

  public:
    Log() = delete;

    Log(LogType type = LogType(), float time = 0.f, const char *text = "") : m_type(type), m_time(time), m_text(text) {}

    Log(const char *log_type, const char *log_type_text, ImVec4 color, float time, const char *text)
        : Log(LogType(log_type, log_type_text, color), time, std::string(text).c_str()) {}

    inline const char *getLogType() { return this->m_type.getLogType(); }

    inline LogType getLogTypeObject() { return this->m_type; }

    inline const char *getLogTypeText() { return this->m_type.getLogTypeText(); }

    inline ImVec4 getLogTypeColor() { return this->m_type.getColor(); }

    inline float getLogTime() { return this->m_time; }

    inline const char *getLogText() { return this->m_text; }

    ~Log() = default;
};
