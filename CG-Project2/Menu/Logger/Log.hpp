#pragma once

#include "../../LibCustomGui.hpp"

enum LogType {
    GENERAL_EVENT,
    SELECT_WARNING,
    ERROR,
    EMPTY
};

inline ImVec4 getLogColor(LogType t) {
    switch (t) {
    case LogType::GENERAL_EVENT: {
        return ImVec4(0, 0.8, 0.01, 1);
    }
    case LogType::SELECT_WARNING: {
        return ImVec4(1, 1, 0, 1);
    }
    case LogType::ERROR: {
        return ImVec4(1, 0, 0, 1);
    }
    default:
        return ImVec4(1, 1, 1, 1);
    }
}

inline std::string getLogLongText(LogType t) {
    switch (t) {
    case LogType::GENERAL_EVENT: {
        return "Event";
    }
    case LogType::SELECT_WARNING: {
        return "Select W";
    }
    case LogType::ERROR: {
        return "Error";
    }
    default:
        return "Empty";
    }
}

inline std::string getLogText(LogType t) {
    switch (t) {
    case LogType::GENERAL_EVENT: {
        return "EVENT";
    }
    case LogType::SELECT_WARNING: {
        return "WARNING";
    }
    case LogType::ERROR: {
        return "ERROR";
    }
    default:
        return "";
    }
}
