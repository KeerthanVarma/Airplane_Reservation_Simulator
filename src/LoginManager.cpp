#include "../include/LoginManager.h"
#include "../include/VoiceAssistant.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

bool isValidAadhaar(const std::string& aadhaar) {
    return aadhaar.length() == 12 && std::all_of(aadhaar.begin(), aadhaar.end(), ::isdigit);
}

bool LoginManager::loginAsPassenger(std::string& aadhaar) {
    std::cout << "Enter Aadhaar Number (12-digit): ";
    std::cin >> aadhaar;
    if (!isValidAadhaar(aadhaar)) {
        std::cout << "Invalid Aadhaar number.\n";
        speak("Invalid Aadhaar number.");
        return false;
    }
    return true;
}

bool LoginManager::loginAsAdmin() {
    std::string inputUsername, inputPassword;

    std::cout << "Enter Admin Username: ";
    std::cin >> inputUsername;
    std::cout << "Enter Admin Password: ";
    std::cin >> inputPassword;

    std::ifstream file("admin_credentials.txt");
    if (!file.is_open()) {
        std::cout << "Could not open admin_credentials.txt\n";
        speak("Login failed.");
        return false;
    }

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string storedUsername, storedPassword;
        getline(ss, storedUsername, ',');
        getline(ss, storedPassword);

        if (inputUsername == storedUsername && inputPassword == storedPassword) {
            return true;
        }
    }

    std::cout << "Invalid username or password.\n";
    speak("Invalid admin credentials.");
    return false;
}
