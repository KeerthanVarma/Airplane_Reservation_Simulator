#include "include/BookingSystem.h"
#include "include/Flight.h"
#include "include/Passenger.h"
#include "include/RouteManager.h"
#include "include/Admin.h"
#include "include/pricing.h"
#include "include/VoiceAssistant.h"
#include "include/BoardingPass.h"
#include "include/LoginManager.h"

#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <bits/stdc++.h>
void clearInputStream() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getValidatedInt(const std::string& prompt, int min = INT_MIN, int max = INT_MAX) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < min || value > max) {
            std::cout << "Invalid input. Enter a valid number";
            if (min != INT_MIN && max != INT_MAX)
                std::cout << " between " << min << " and " << max;
            std::cout << ".\n";
            speak("Invalid input.");
            clearInputStream();
        } else {
            clearInputStream();
            return value;
        }
    }
}

char getValidatedChar(const std::string& prompt, const std::string& validChars) {
    char ch;
    while (true) {
        std::cout << prompt;
        std::cin >> ch;
        ch = toupper(ch);
        if (validChars.find(ch) != std::string::npos) {
            clearInputStream();
            return ch;
        } else {
            std::cout << "Invalid input. Enter one of: " << validChars << "\n";
            speak("Invalid input.");
            clearInputStream();
        }
    }
}

std::string getValidatedAadhaar() {
    std::string aadhaar;
    while (true) {
        std::cout << "Enter Aadhaar Number (12-digit number): ";
        std::cin >> aadhaar;
        if (!isValidAadhaar(aadhaar)) {
            std::cout << "Invalid Aadhaar. Must be 12 digits.\n";
            speak("Invalid Aadhaar.");
            clearInputStream();
        } else {
            return aadhaar;
        }
    }
}

int main() {
    speak("Welcome to the Domestic Airline Booking System.");

    BookingSystem system;
    RouteManager routeManager;
    std::string currentAadhaar;
    int loginChoice;
    system.loadFlightsFromFile("flights.txt");
    system.loadDataFromFile("bookings.txt");

    std::cout << "Login as:\n1. Admin\n2. Passenger\nEnter choice (1 or 2): ";
    std::cin >> loginChoice;
    clearInputStream();

    if (loginChoice == 1) {
        if (!LoginManager::loginAsAdmin()) {
            std::cout << "Admin login failed. Exiting.\n";
            speak("Admin login failed.");
            return 0;
        }
        std::cout << "Admin login successful.\n";
        speak("Admin login successful.");

        Admin admin;
        admin.showMenu(system); 
        system.saveDataToFile("bookings.txt"); // Go directly to admin panel
        return 0;
    }

    if (loginChoice != 2 || !LoginManager::loginAsPassenger(currentAadhaar)) {
        std::cout << "Passenger login failed. Exiting.\n";
        speak("Passenger login failed.");
        return 0;
    }
    std::cout << "Passenger login successful.\n";
    speak("Passenger login successful.");

    // Passenger flow
    std::string src, dst, date;
    std::cout << "Enter Source City: ";
    std::cin >> src;
    std::cout << "Enter Destination City: ";
    std::cin >> dst;
    std::cout << "Enter Date (YYYY-MM-DD): ";
    std::cin >> date;

    src = RouteManager::normalize(src);
    dst = RouteManager::normalize(dst);

    auto flights = routeManager.getFlightOptionsByTime(src, dst, RouteManager::TimeSlot::Morning);
    if (flights.empty()) {
        std::cout << "No flights available.\n";
        return 0;
    }

    for (size_t i = 0; i < std::min<size_t>(9, flights.size()); ++i) {
        std::string id = flights[i].first;
        std::string time = flights[i].second;
        int baseFare = getBaseFare(src, dst);
        Flight flight(id, src, dst, date, time, 40, 6, baseFare);
        system.addFlight(flight);
        system.saveFlightsToFile("flights.txt");

    }

    while (true) {
        std::cout << "\n--- MAIN MENU ---\n";
        std::cout << "1. View Flights\n";
        std::cout << "2. Book Ticket\n";
        std::cout << "3. Cancel Ticket\n";
        std::cout << "4. View Booking History\n";
        std::cout << "5. Generate Boarding Pass\n";
        std::cout << "6. Logout\n";

        int choice = getValidatedInt("Enter your choice (1-6): ", 1, 6);

        if (choice == 1) {
            system.listFlights();

        } else if (choice == 2) {
            std::string flightID;
            std::cout << "Enter Flight ID: ";
            std::cin >> flightID;

            const Flight* f = nullptr;
            for (auto& fl : system.getFlights()) {
                if (fl.flightID == flightID) {
                    f = &fl;
                    break;
                }
            }

            if (!f) {
                std::cout << "Flight not found.\n";
                speak("Flight not found.");
                continue;
            }

            f->displaySeats();

            std::string tempName;
            int tempAge;
            char tempGender;
            int tempRow;
            char tempColChar;

            while (true) {
                std::cout << "Enter Passenger Name: ";
                std::cin >> tempName;
                tempAge = getValidatedInt("Enter Age: ", 1, 120);
                tempGender = getValidatedChar("Enter Gender (M/F): ", "MF");
                tempRow = getValidatedInt("Enter Seat Row (1-40): ", 1, 40);
                tempColChar = getValidatedChar("Enter Seat Column (A-F): ", "ABCDEF");

                std::string confirm;
                std::cout << "You entered:\n";
                std::cout << "  Name: " << tempName << "\n";
                std::cout << "  Age: " << tempAge << "\n";
                std::cout << "  Gender: " << tempGender << "\n";
                std::cout << "  Seat: " << tempRow << tempColChar << "\n";
                std::cout << "Confirm booking details? (yes/back): ";
                std::cin >> confirm;

                if (confirm == "yes") break;
                else std::cout << "Re-enter your details.\n";
            }

            int tempCol = toupper(tempColChar) - 'A';

            if (!f->isSeatAvailable(tempRow - 1, tempCol)) {
                std::cout << "Seat is already booked. Please choose another seat.\n";
                speak("Seat is already booked.");
                continue;
            }

            Passenger p(tempName, currentAadhaar, tempAge, tempGender);
            if (system.bookTicket(flightID, p, tempRow - 1, tempCol)) {
                std::cout << "Booking successful.\n";
                speak("Booking successful.");
            } else {
                std::cout << "Booking failed.\n";
                speak("Booking failed.");
            }

        } else if (choice == 3) {
            std::string flightID;
            std::cout << "Enter Flight ID to cancel: ";
            std::cin >> flightID;

            std::string confirm;
            std::cout << "Are you sure you want to cancel this ticket? (yes/no): ";
            std::cin >> confirm;
            if (confirm != "yes") {
                std::cout << "Cancellation aborted.\n";
                continue;
            }

            system.cancelTicket(flightID, currentAadhaar);

        } else if (choice == 4) {
            system.viewPassengerHistory(currentAadhaar);

        } else if (choice == 5) {
            std::string flightID;
            std::cout << "Enter Flight ID: ";
            std::cin >> flightID;

            const auto& flights = system.getFlights();
            const Flight* f = nullptr;
            for (const auto& fl : flights) {
                if (fl.flightID == flightID) {
                    f = &fl;
                    break;
                }
            }

            if (!f) {
                std::cout << "Flight not found.\n";
                continue;
            }

            auto seat = system.getPassengerSeat(flightID, currentAadhaar);
            if (seat.first != -1 && seat.second != -1) {
                const auto& bookings = system.getBookings().at(flightID);
                for (const auto& p : bookings) {
                    if (p.passportNumber == currentAadhaar) {
                        BoardingPass::generate(p, *f, seat.first, seat.second);
                        break;
                    }
                }
            } else {
                std::cout << "No boarding pass found.\n";
                speak("Boarding pass not found.");
            }

        } else if (choice == 6) {
            std::cout << "Logged out successfully. Goodbye!\n";
            speak("Logged out. Thank you.");
            system.saveDataToFile("bookings.txt");
            break;
        }
    }

    return 0;
}
