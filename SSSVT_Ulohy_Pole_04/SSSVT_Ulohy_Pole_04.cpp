#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <Windows.h>
#include <string>

#pragma warning(disable : 4996)

bool hasWindows() {
#if defined(WIN32) || defined(WIN32) || defined(__WIN32) && !defined(__CYGWIN_)
    return true;
#else
    return false;
#endif
}

std::string fix_date(const std::string& date_str) {
    if (date_str.size() != 4)
        return "invalid length";
    else if (!(!date_str.empty() && std::all_of(date_str.begin(), date_str.end(), ::isdigit)))
        return "not numeric";

    int day = std::stoi(date_str.substr(0, 2));
    int month = std::stoi(date_str.substr(2, 4));
    return std::to_string(day) + ". " + std::to_string(month);
};

std::wstring utf8_to_wide(const std::string& str)
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}

std::chrono::system_clock::duration duration_since_midnight() {
    auto now = std::chrono::system_clock::now();

    time_t tnow = std::chrono::system_clock::to_time_t(now);
    tm* date = std::localtime(&tnow);
    date->tm_hour = 0;
    date->tm_min = 0;
    date->tm_sec = 0;
    auto midnight = std::chrono::system_clock::from_time_t(std::mktime(date));

    return now - midnight;
}

int get_hours() {
    std::cout << "Enter hour (0-23, not AM/PM): ";
    std::string hour_s;
    std::cin >> hour_s;
    
    return std::stoi(hour_s);
}

int get_minutes() {
    std::cout << "Enter minutes (0-59): ";
    std::string minutes_s;
    std::cin >> minutes_s;

    return std::stoi(minutes_s);
}

void Run() {
    //send request
    std::string response_text = cpr::Get(cpr::Url{ "https://svatky.adresa.info/json?lang=cs" }).text;

    //parse json request response
    nlohmann::json parsed = nlohmann::json::parse(response_text, nullptr, false);

    //discard check
    if (parsed.is_discarded()) return false;

    //declare variables
    int u_hours = 0;
    int u_minutes = 0;

    //get announce time
    u_hours = get_hours();
    u_minutes = get_minutes();

    //print info
    std::cout << "PAUSE - Show Window Again" << "\n";
    std::cout << "DEL - Reset app and enter time again" << "\n";

    //print hide time remaining
    for (short i = 5; i > 0; i--) {
        std::cout << "Hiding window in " << i << "..." << "\n";
        Sleep(1000);
    }

    //hide console window
    bool show = false;
    HWND window;
    AllocConsole();
    window = GetConsoleWindow();
    ShowWindow(window, show);

    while (true) {
        //get time since midnight
        auto since_midnight = duration_since_midnight();
        auto hours = std::chrono::duration_cast<std::chrono::hours>(since_midnight);
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(since_midnight - hours);
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(since_midnight - hours - minutes);

        //if user clicks on PAUSE button - shows up window
        if (GetAsyncKeyState(VK_PAUSE) & 0x13) {
            show = !show;
            ShowWindow(window, show);
            std::cout << "Welcome back, would you like to reset announce time?" << "\n";
        }

        //reset announce time if window is opened
        if ((GetAsyncKeyState(VK_DELETE) & 0x13) && window) {
            for (short i = 5; i > 0; i--) {
                std::cout << "Reseting announce time in " << i << "..." << "\n";
                Sleep(1000);
            }
            system(hasWindows() ? "cls" : "clear");
            Run();
            break;
        }

        //checking if current time equals users announce time every second
        if (hours.count() == u_hours && minutes.count() == u_minutes && seconds.count() == 0) {
            for (const auto& var : parsed) {
                std::string msg = "Today " + fix_date(var["date"].get<std::string>()) + " is a " + var["name"].get<std::string>() + "s name day, wish her/him luck";
                MessageBoxW(0, utf8_to_wide(msg).c_str(), L"name day announcer", 0);
            }
        }       
        Sleep(1000);
    }
}

int main(int argc, char *argv[])
{
    //call main function
    Run();
    std::cin.get();

    return 0;
}