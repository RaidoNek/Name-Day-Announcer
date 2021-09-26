#pragma once
#include <string>
#include <windows.h>
#include <chrono>

namespace utilities {
    bool hasWindows();
    bool isNumeric(const std::string& str);
    std::wstring utf8ToWide(const std::string& str);
    std::chrono::system_clock::duration durationSinceMidnight();
};

