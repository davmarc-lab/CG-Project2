#pragma once

#include "../../LibCustomGui.hpp"

enum LogType {
    SELECT_WARNING,
    ERROR,
    EMPTY
};

inline ImVec4 getLogColor(LogType t) {
    switch (t) {
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

inline std::string getLogText(LogType t) {
    switch (t) {
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
