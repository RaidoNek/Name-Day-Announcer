#include "utilities.h"

bool utilities::hasWindows() {
    #if defined(WIN32) || defined(WIN32) || defined(__WIN32) && !defined(__CYGWIN_)
    return true;
    #else
    return false;
    #endif
}

bool utilities::isNumeric(const std::string& str) {
    for (const auto& var : str) {
        if (!isdigit(var)) return false;
    }
    return true;
}

std::wstring utilities::utf8ToWide(const std::string& str)
{
    const int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}

std::chrono::system_clock::duration utilities::durationSinceMidnight() {
    const auto now = std::chrono::system_clock::now();

    const time_t tnow = std::chrono::system_clock::to_time_t(now);
    tm* date = std::localtime(&tnow);
    date->tm_hour = 0;
    date->tm_min = 0;
    date->tm_sec = 0;
    const auto midnight = std::chrono::system_clock::from_time_t(std::mktime(date));

    return now - midnight;
}