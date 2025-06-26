#include "../include/Pricing.h"
#include "../include/VoiceAssistant.h"
#include <string>

int getBaseFare(const std::string &src, const std::string &dst) {
    if ((src == "mumbai" && dst == "pune") || (src == "pune" && dst == "mumbai") ||
        (src == "chennai" && dst == "bengaluru") || (src == "bengaluru" && dst == "chennai")) {
        return 3000;
    }

    if ((src == "mumbai" && dst == "goa") || (src == "goa" && dst == "mumbai") ||
        (src == "ahmedabad" && dst == "goa") || (src == "goa" && dst == "ahmedabad") ||
        (src == "ahmedabad" && dst == "mumbai") || (src == "mumbai" && dst == "ahmedabad")) {
        return 4500;
    }

    if ((src == "ahmedabad" && dst == "bengaluru") || (src == "bengaluru" && dst == "ahmedabad") ||
        (src == "ahmedabad" && dst == "chennai") || (src == "chennai" && dst == "ahmedabad") ||
        (src == "delhi" && dst == "chennai") || (src == "chennai" && dst == "delhi") ||
        (src == "delhi" && dst == "bengaluru") || (src == "bengaluru" && dst == "delhi") ||
        (src == "delhi" && dst == "kolkata") || (src == "kolkata" && dst == "delhi") ||
        (src == "hyderabad" && dst == "ahmedabad") || (src == "ahmedabad" && dst == "hyderabad")) {
        return 6000;
    }

    if ((src == "delhi" && dst == "kochi") || (src == "kochi" && dst == "delhi") ||
        (src == "kolkata" && dst == "kochi") || (src == "kochi" && dst == "kolkata") ||
        (src == "kochi" && dst == "jaipur") || (src == "jaipur" && dst == "kochi") ||
        (src == "kochi" && dst == "lucknow") || (src == "lucknow" && dst == "kochi")) {
        return 7500;
    }

    return 4000; // Default fare
}

double Pricing::calculateFare(int baseFare, int totalSeats, int bookedSeats) {
    double occupancyRate = static_cast<double>(bookedSeats) / totalSeats;

    if (occupancyRate < 0.3) {
        return baseFare;
    } else if (occupancyRate < 0.6) {
        return baseFare * 1.25;
    } else if (occupancyRate < 0.9) {
        return baseFare * 1.5;
    } else {
        return baseFare * 2.0;
    }
}
