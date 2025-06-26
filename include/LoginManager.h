#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <string>

class LoginManager {
public:
    static bool loginAsAdmin();
    static bool loginAsPassenger(std::string& aadhaar);
};

bool isValidAadhaar(const std::string& aadhaar);  // You already have this.

#endif
