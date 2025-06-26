#ifndef BOOKINGSYSTEM_H
#define BOOKINGSYSTEM_H

#include "Flight.h"
#include "Passenger.h"
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <string>
#include <utility>

// Struct to keep track of booking times
struct BookingRecord {
    std::string flightID;
    std::string bookingTime;
};

class BookingSystem {
private:
    std::vector<Flight> flights;

    std::map<std::string, std::vector<Passenger>> bookings;
    std::map<std::string, std::unordered_set<std::string>> bookedPassports;
    std::map<std::string, std::vector<BookingRecord>> bookingHistory;
    std::map<std::string, std::vector<BookingRecord>> passengerHistory;

    // flightID → [(aadhaar, (row, col))]
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::pair<int, int>>>> passengerSeatMap;

    struct AgeComparator {
        bool operator()(const Passenger& a, const Passenger& b) {
            return a.age < b.age;  // Older passengers first
        }
    };

    std::map<std::string, std::priority_queue<Passenger, std::vector<Passenger>, AgeComparator>> waitlist;
    std::map<std::string, std::queue<Passenger>> boardingQueue;

public:
    // --- Core Passenger Functionalities ---
    void addFlight(const Flight& f);
    void listFlights() const;
    bool bookTicket(const std::string& flightID, const Passenger& p, int row, int col);
    void cancelTicket(const std::string& flightID, const std::string& passportNumber);

    // --- Passenger History & Search ---
    void viewPassengerHistory(const std::string& passportNumber) const;
    void viewBookingHistory(const std::string& passportNumber) const;
    Passenger* findPassenger(const std::string& flightID, const std::string& aadhaar);

    // --- Seat Info for Boarding Pass ---
    std::pair<int, int> getPassengerSeat(const std::string& flightID, const std::string& aadhaar) const;
    const std::unordered_map<std::string, std::vector<std::pair<std::string, std::pair<int, int>>>>& getPassengerSeatMap() const;

    // --- Admin Accessors ---
    const std::vector<Flight>& getFlights() const { return flights; }
    const std::map<std::string, std::vector<Passenger>>& getBookings() const { return bookings; }
    const std::map<std::string, std::queue<Passenger>>& getBoardingQueue() const { return boardingQueue; }
    const std::map<std::string, std::priority_queue<Passenger, std::vector<Passenger>, AgeComparator>>& getWaitlist() const { return waitlist; }

    // --- File Persistence ---
    void saveDataToFile(const std::string& filename) const;
    void loadDataFromFile(const std::string& filename);
    void saveFlightsToFile(const std::string& filename) const;   // ✅ Added
    void loadFlightsFromFile(const std::string& filename);       // ✅ Added
};

#endif
