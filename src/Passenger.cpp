#include "../include/Passenger.h"
#include "../include/VoiceAssistant.h"
Passenger::Passenger(std::string n, std::string pno, int a, char g)
    : name(n), passportNumber(pno), age(a), gender(g) {}
