#ifndef FLIGHT_H
#define FLIGHT_H

#include <iostream>
#include <vector>
#include <string>

class Flight {
public:
    // Flight details
    std::string flightID;
    std::string source, destination;
    std::string date, time;

    int rows, cols;
    int totalSeats;
    int bookedSeats;
    int baseFare;

    std::vector<std::vector<bool>> seatMatrix;

    // Constructor
    Flight(std::string id, std::string src, std::string dst,
           std::string dt, std::string tm, int r, int c, int fare);

    // Seat operations
    void displaySeats() const;
    bool isSeatAvailable(int row, int col) const;
    void bookSeat(int row, int col);

    // Admin statistics support
    int getTotalRows() const;
    int getTotalCols() const;
    int getBookedSeats() const;
    int getTotalSeats() const;
    int getRevenue() const;
    double getOccupancyRate() const;
};

#endif
