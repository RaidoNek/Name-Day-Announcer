#include "NameDay.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "utilities/utilities.h"

void NameDay::Run() {
    //initialize object class

    //get country code for name day
    const std::string country_code = getCountry();

    //declare variables
    const unsigned short u_hours = getHours();
    const unsigned short u_minutes = getMinutes();

    //print info
    std::cout << "PAUSE - Show Window Again" << "\n";
    std::cout << "DEL - Reset app and enter time again (only if u have a window enabled)" << "\n";

    //print hide time remaining
    for (short i = 5; i > 0; i--) {
        std::cout << "Hiding window in " << i << "..." << "\n";
        Sleep(1000);
    }

    //hide console window
    bool show = false;
    HWND window = GetConsoleWindow();
    AllocConsole();
    ShowWindow(window, show);

    while (true) {
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
            system(utilities::hasWindows() ? "cls" : "clear");
            Run();
            break;
        }

        //checking if current time equals users announce time every second
        ShowNameDay(u_hours, u_minutes, country_code);
        Sleep(1000);
    }
}

void NameDay::ShowNameDay(unsigned short u_hours, unsigned short u_minutes, const std::string& country_code) {

    //get time since midnight
    const auto since_midnight = utilities::durationSinceMidnight();
    const auto hours = std::chrono::duration_cast<std::chrono::hours>(since_midnight);
    const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(since_midnight - hours);
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(since_midnight - hours - minutes);

    if (hours.count() == u_hours && minutes.count() == u_minutes && seconds.count() == 0) {
        //send request
        const cpr::Response r = cpr::Post(cpr::Url{ "https://nameday.abalin.net/today" }, cpr::Parameters{ {"country", country_code} });
        const std::string response_text = r.text;

        //parse json request response
        const nlohmann::json parsed = nlohmann::json::parse(response_text, nullptr, false);

        //discard check
        if (parsed.is_discarded()) exit(0);

        for (const auto& var : parsed) {
            //get value
            const std::string value = var["namedays"][country_code].get<std::string>();

            //get if there is one person, or multiple
            const bool contains = (value.find(",") != std::string::npos);

            //get name... if valid -> print name, if not -> print "No one"
            const std::string name = ((value == "n\/a") ? "No one" : value);

            //get spelling -> if str contains , -> it definitely contains multiple names
            const std::string spelling = ((contains) ? " have" : " has");
            const std::string spelling2 = ((contains) ? "them" : "her/him");

            const std::string msg = name + spelling + " a name day today, wish " + spelling2 + " luck";
            MessageBoxW(0, utilities::utf8ToWide(msg).c_str(), L"name day announcer", 0);
            break;
        }
    }
}

bool NameDay::isNumeric(const std::string& str) {
    for (const auto& var : str) {
        if (!isdigit(var)) return false;
    }
    return true;
}

unsigned short NameDay::getHours() {
    std::cout << "Enter an hour (0-23, not AM/PM): ";
    std::string hour_s;
    std::cin >> hour_s;

    if (!(isNumeric(hour_s))) {
        std::cout << "Invalid input, try again." << "\n";
        getHours();
    }

    const unsigned short input = std::stoi(hour_s);
    if (input > 23 || input < 0) {
        std::cout << "Invalid input, try again." << "\n";
        getHours();
    }



    return input;
}

unsigned short NameDay::getMinutes() {
    std::cout << "Enter a minute (0-59): ";
    std::string minutes_s;
    std::cin >> minutes_s;

    if (!(isNumeric(minutes_s))) {
        std::cout << "Invalid input, try again." << "\n";
        getMinutes();
    }

    const unsigned short input = std::stoi(minutes_s);
    if (input > 59 || input < 0) {
        std::cout << "Invalid input, try again." << "\n";
        getMinutes();
    }

    return input;
}

std::string NameDay::getCountry() {
    //declare supported country codes
    //nevim dopice, ma tu byt std::string??? nejde mi tu dat constexpr :(
    const std::string codes[] = {
        "at",
        "bg",
        "cz",
        "de",
        "dk",
        "ee",
        "es",
        "fi",
        "fr",
        "gr",
        "hr",
        "hu",
        "it",
        "lt",
        "lv",
        "pl",
        "ru",
        "se",
        "sk",
        "us"
    };

    //print info messages
    std::cout << "Hello, first enter country code for which you want to announce name days" << "\n";
    std::cout << "Supported country codes are: ";

    //declare length, dynamically show all supported country codes
    const unsigned short length = (sizeof(codes) / sizeof(codes[0]));
    for (unsigned short i = 0; i < length; i++) {
        std::cout << codes[i] << ((i == (length - 1)) ? "" : " | ");
    }

    //let user to enter code
    std::cout << "\n";
    std::cout << "Enter country code: ";

    //get country code and validate
    std::string country_s;
    std::cin >> country_s;

    for (unsigned short i = 0; i < length; i++) {
        //if (strcmp(country_s.c_str(), codes[i].c_str()) == 0) {
        if (country_s == codes[i]) {
            return country_s;
        }
    }

    //User entered invalid country code
    std::cout << "That's not a supported country code" << "\n";
    for (unsigned short i = 3; i > 0; i--) {
        std::cout << "Reseting app in " << i << "..." << "\n";
        Sleep(1000);
    }
    system(utilities::hasWindows() ? "cls" : "clear");
    getCountry();
}

