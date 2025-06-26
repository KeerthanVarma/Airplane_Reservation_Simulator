#include "../include/BookingSystem.h"
#include "../include/Pricing.h"
#include "../include/VoiceAssistant.h"
#include "../include/Flight.h"
#include "../include/Pricing.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <ctime>

// Utility to parse YYYY-MM-DD
std::tm parseDate(const std::string& dateStr) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return tm;
}

// Refund Calculation
double calculateRefund(const std::string& flightDate, double originalFare) {
    std::tm flight_tm = parseDate(flightDate);
    std::time_t now = std::time(nullptr);
    std::tm* current_tm = std::localtime(&now);

    std::time_t flight_time = std::mktime(&flight_tm);
    std::time_t current_time = std::mktime(current_tm);

    double secondsLeft = std::difftime(flight_time, current_time);
    int daysLeft = static_cast<int>(secondsLeft / (60 * 60 * 24));

    if (daysLeft > 7) return originalFare;
    else if (daysLeft >= 1) return originalFare * 0.5;
    else return 0.0;
}

void BookingSystem::addFlight(const Flight &f) {
    flights.push_back(f);
}

void BookingSystem::listFlights() const {
    std::cout << "\nAvailable Flights:\n";
    for (const auto &f : flights) {
        std::cout << "ID: " << f.flightID
                  << " | From: " << f.source
                  << " | To: " << f.destination
                  << " | Date: " << f.date
                  << " | Time: " << f.time << "\n";
    }
}

bool BookingSystem::bookTicket(const std::string &flightID, const Passenger &p, int row, int col) {
    for (auto &f : flights) {
        if (f.flightID == flightID) {
            if (f.isSeatAvailable(row, col)) {
                f.bookSeat(row, col);
                bookings[flightID].push_back(p);

                int baseFare = getBaseFare(f.source, f.destination);
                double finalFare = Pricing::calculateFare(baseFare, f.totalSeats, f.bookedSeats);

                auto now = std::chrono::system_clock::now();
                std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
                std::stringstream ss;
                ss << std::put_time(std::localtime(&timeNow), "%Y-%m-%d %H:%M:%S");

                BookingRecord record{flightID, ss.str()};
                passengerHistory[p.passportNumber].push_back(record);
                passengerSeatMap[flightID].push_back({ p.passportNumber, {row, col} });

                std::cout << "Ticket Fare: ₹" << finalFare << "\n";
                speak("Your ticket fare is rupees " + std::to_string((int)finalFare));
                f.displaySeats();
                return true;
                saveDataToFile("bookings.txt");

            } else {
                std::cout << "Seat already booked.\n";
                return false;
            }
        }
    }
    std::cout << "Flight not found.\n";
    return false;
}

void BookingSystem::cancelTicket(const std::string &flightID, const std::string &passportNumber) {
    auto it = bookings.find(flightID);
    if (it != bookings.end()) {
        auto &passengerList = it->second;
        for (auto pIt = passengerList.begin(); pIt != passengerList.end(); ++pIt) {
            if (pIt->passportNumber == passportNumber) {
                for (auto &f : flights) {
                    if (f.flightID == flightID) {
                        f.bookedSeats--;

                        double originalFare = Pricing::calculateFare(
                            getBaseFare(f.source, f.destination),
                            f.totalSeats,
                            f.bookedSeats
                        );

                        double refund = calculateRefund(f.date, originalFare);
                        std::cout << "Refund Amount: ₹" << refund << "\n";
                        std::cout << "Cancellation Fee: ₹" << originalFare - refund << "\n";

                        passengerList.erase(pIt);
                        return;
                    }
                }
            }
        }
    }
    std::cout << "Ticket not found.\n";
}

void BookingSystem::viewPassengerHistory(const std::string& passportNumber) const {
    std::cout << "\n--- Booking History for Aadhaar: " << passportNumber << " ---\n";
    bool found = false;

    for (const auto& booking : bookings) {
        const std::string& flightID = booking.first;
        const auto& passengerList = booking.second;

        for (const auto& p : passengerList) {
            if (p.passportNumber == passportNumber) {
                found = true;

                auto it = std::find_if(flights.begin(), flights.end(), [&](const Flight& f) {
                    return f.flightID == flightID;
                });

                if (it != flights.end()) {
                    const Flight& f = *it;
                    auto seat = getPassengerSeat(flightID, passportNumber);
                    char seatCol = 'A' + seat.second;
                    std::cout << "Flight ID: " << f.flightID
                              << " | Route: " << f.source << " -> " << f.destination
                              << " | Date: " << f.date
                              << " | Time: " << f.time
                              << " | Seat: " << (seat.first + 1) << seatCol << "\n";
                }
            }
        }
    }

    if (!found) {
        std::cout << "No bookings found for this Aadhaar.\n";
    }
}

void BookingSystem::viewBookingHistory(const std::string& passportNumber) const {
    auto it = passengerHistory.find(passportNumber);
    if (it == passengerHistory.end() || it->second.empty()) {
        std::cout << "No booking history found for this Aadhaar.\n";
        return;
    }

    std::cout << "\nBooking History:\n";
    for (const auto& record : it->second) {
        std::cout << "Flight ID: " << record.flightID << " | Booked On: " << record.bookingTime << "\n";
    }
}

Passenger* BookingSystem::findPassenger(const std::string& flightID, const std::string& aadhaar) {
    auto it = bookings.find(flightID);
    if (it != bookings.end()) {
        for (auto& p : it->second) {
            if (p.passportNumber == aadhaar) {
                return &p;
            }
        }
    }
    return nullptr;
}

std::pair<int, int> BookingSystem::getPassengerSeat(const std::string& flightID, const std::string& aadhaar) const {
    auto it = passengerSeatMap.find(flightID);
    if (it != passengerSeatMap.end()) {
        for (const auto& pair : it->second) {
            if (pair.first == aadhaar) {
                return pair.second;
            }
        }
    }
    return {-1, -1};
}

const std::unordered_map<std::string, std::vector<std::pair<std::string, std::pair<int, int>>>>& BookingSystem::getPassengerSeatMap() const {
    return passengerSeatMap;
}

// Save booking details to a file
void BookingSystem::saveDataToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error: Unable to open file for writing.\n";
        return;
    }

    for (auto it = bookings.begin(); it != bookings.end(); ++it) {
        const std::string& flightID = it->first;
        const std::vector<Passenger>& passengers = it->second;

        for (const auto& p : passengers) {
            std::pair<int, int> seat = getPassengerSeat(flightID, p.passportNumber);
            out << flightID << "," << p.name << "," << p.passportNumber << "," << p.age << "," << p.gender
                << "," << seat.first << "," << seat.second << "\n";
        }
    }

    out.close();
}


// Load booking details from a file (optional enhancement)
void BookingSystem::loadDataFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string flightID, name, aadhaar, ageStr, genderStr, rowStr, colStr;
        std::getline(ss, flightID, ',');
        std::getline(ss, name, ',');
        std::getline(ss, aadhaar, ',');
        std::getline(ss, ageStr, ',');
        std::getline(ss, genderStr, ',');
        std::getline(ss, rowStr, ',');
        std::getline(ss, colStr, ',');

        int age = std::stoi(ageStr);
        char gender = genderStr[0];
        int row = std::stoi(rowStr);
        int col = std::stoi(colStr);

        Passenger p(name, aadhaar, age, gender);
        bookings[flightID].push_back(p);
        passengerSeatMap[flightID].push_back({ aadhaar, {row, col} });

        for (auto& f : flights) {
            if (f.flightID == flightID) {
                f.bookSeat(row, col);
                break;
            }
        }
    }
    in.close();
}
void BookingSystem::saveFlightsToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    for (const auto& f : flights) {
        out << f.flightID << "," << f.source << "," << f.destination << ","
            << f.date << "," << f.time << "," << f.rows << "," << f.cols << "," << f.baseFare << "\n";
    }
    out.close();
}
void BookingSystem::loadFlightsFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return;

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string id, src, dst, date, time, rowsStr, colsStr, fareStr;
        std::getline(ss, id, ',');
        std::getline(ss, src, ',');
        std::getline(ss, dst, ',');
        std::getline(ss, date, ',');
        std::getline(ss, time, ',');
        std::getline(ss, rowsStr, ',');
        std::getline(ss, colsStr, ',');
        std::getline(ss, fareStr, ',');

        int rows = std::stoi(rowsStr);
        int cols = std::stoi(colsStr);
        int fare = std::stoi(fareStr);

        flights.emplace_back(id, src, dst, date, time, rows, cols, fare);
    }
    in.close();
}


