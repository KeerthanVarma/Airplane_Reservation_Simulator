#include "../include/RouteManager.h"
#include "../include/VoiceAssistant.h"
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iomanip>

RouteManager::RouteManager() {
    generateRoutes();
}

std::string RouteManager::normalize(const std::string &city) {
    std::string result = city;
    result[0] = toupper(result[0]);
    for (size_t i = 1; i < result.size(); ++i)
        result[i] = tolower(result[i]);
    return result;
}

std::string RouteManager::generateRandomTime(TimeSlot slot) {
    int startHour, endHour;
    switch (slot) {
        case TimeSlot::Morning:
            startHour = 6; endHour = 11;
            break;
        case TimeSlot::Afternoon:
            startHour = 12; endHour = 17;
            break;
        case TimeSlot::Evening:
            startHour = 18; endHour = 22;
            break;
    }

    int hour = startHour + rand() % (endHour - startHour + 1);
    int minute = rand() % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hour << ":"
        << std::setw(2) << std::setfill('0') << minute;
    return oss.str();
}

void RouteManager::generateRoutes() {
    srand(static_cast<unsigned>(time(nullptr)));
    std::vector<std::string> cities = {
        "Mumbai", "Delhi", "Chennai", "Kolkata", "Bengaluru",
        "Hyderabad", "Ahmedabad", "Kochi", "Goa", "Jaipur"
    };

    for (const auto &src : cities) {
        for (const auto &dst : cities) {
            if (src == dst) continue;

            std::pair<std::string, std::string> route = {src, dst};
            for (int i = 1; i < 10; ++i) {
                std::string code = src.substr(0, 3) + dst.substr(0, 3) + std::to_string(i);
                std::string time = generateRandomTime(TimeSlot::Morning);
                routes[route].emplace_back(code, time);
            }
        }
    }
}

std::string RouteManager::getNextFlightID(const std::string &src, const std::string &dst, const std::string &time) {
    std::pair<std::string, std::string> key = {src, dst};
    auto &options = routes[key];
    for (const auto &opt : options) {
        if (opt.second == time) return opt.first;
    }
    return "UNAVAILABLE";
}

std::vector<std::pair<std::string, std::string>> RouteManager::getFlightOptionsByTime(
    const std::string &src,
    const std::string &dst,
    TimeSlot slot
) {
    std::pair<std::string, std::string> key = {src, dst};
    auto options = routes[key];

    std::sort(options.begin(), options.end(), [](const auto &a, const auto &b) {
        return a.second < b.second; // sort by time
    });

    return options;
}
