#ifndef ROUTEMANAGER_H
#define ROUTEMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <random>
#include <algorithm>

class RouteManager {
public:
    enum class TimeSlot { Morning, Afternoon, Evening };

    RouteManager();

    static std::string normalize(const std::string &city);
    std::string getNextFlightID(const std::string &src, const std::string &dst, const std::string &time);
    std::vector<std::pair<std::string, std::string>> getFlightOptionsByTime(
        const std::string &src,
        const std::string &dst,
        TimeSlot slot
    );

private:
    std::map<std::pair<std::string, std::string>, std::vector<std::pair<std::string, std::string>>> routes;
    std::map<std::string, int> flightCodeCounter;

    void generateRoutes();
    std::string generateRandomTime(RouteManager::TimeSlot slot);
};

#endif
