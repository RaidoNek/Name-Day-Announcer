#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <Windows.h>
#include <codecvt>
#include <string>

auto fix_date = [](std::string date_str) -> std::string {
    if (date_str.size() != 4)
        return "invalid length";
    else if (!(!date_str.empty() && std::all_of(date_str.begin(), date_str.end(), ::isdigit)))
        return "not numeric";

    int day = std::stoi(date_str.substr(0, 2));
    int month = std::stoi(date_str.substr(2, 4));
    return std::to_string(day) + ". " + std::to_string(month);
};

int main()
{
    //send request
    std::string response_text = cpr::Get(cpr::Url{ "https://svatky.adresa.info/json?lang=cs" }).text;

    //parse json request response
    nlohmann::json parsed = nlohmann::json::parse(response_text, nullptr, false);

    //discard check
    if (parsed.is_discarded()) return false;

    //hide console window
    HWND window;
    AllocConsole();
    window = GetConsoleWindow();
    ShowWindow(window, 0); 

    //print info from request
    for (const auto& var : parsed) {
        std::string msg = "Today " + fix_date(var["date"].get<std::string>()) + " is a " + var["name"].get<std::string>() + "s name day, wish her/him luck";
        MessageBoxA(0, msg.c_str(), "name day announcer", 0);
    }

    std::cin.get();

    return 0;
}