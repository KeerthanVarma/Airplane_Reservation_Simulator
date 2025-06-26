#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>

class Passenger {
public:
    std::string name, passportNumber;
    int age;
    char gender;

    Passenger(std::string n, std::string pno, int a, char g);
};

#endif
