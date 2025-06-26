#ifndef PRICING_H
#define PRICING_H
#include <string>
class Pricing {
public:
    // Calculates final ticket price using base fare and occupancy
    static double calculateFare(int baseFare, int totalSeats, int bookedSeats);
    

    
};
int getBaseFare(const std::string &src, const std::string &dst);
#endif
