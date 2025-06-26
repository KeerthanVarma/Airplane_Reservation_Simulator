#include "../include/Flight.h"
#include "../include/VoiceAssistant.h"
#include <iomanip>  // for setw

// Constructor
Flight::Flight(std::string id, std::string src, std::string dst,
               std::string dt, std::string tm, int r, int c, int fare)
    : flightID(id), source(src), destination(dst), date(dt), time(tm),
      rows(r), cols(c), baseFare(fare) {
    totalSeats = rows * cols;
    bookedSeats = 0;
    seatMatrix = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false));
}

// Display seat layout
void Flight::displaySeats() const {
    std::cout << "\nSeat Layout for Flight " << flightID << ":\n";

    // Print column headers (A, B, C, ...)
    std::cout << "    ";
    for (int j = 0; j < cols; ++j) {
        std::cout << " " << static_cast<char>('A' + j) << " ";
    }
    std::cout << "\n";

    // Print seat rows
    for (int i = 0; i < rows; ++i) {
        std::cout << std::setw(2) << i + 1 << "  ";  // Row numbers
        for (int j = 0; j < cols; ++j) {
            std::cout << "[" << (seatMatrix[i][j] ? 'X' : ' ') << "]";
        }
        std::cout << "\n";
    }

    std::cout << "Note: [X] = Booked, [ ] = Available\n";
}

// Check if a seat is available
bool Flight::isSeatAvailable(int row, int col) const {
    return row >= 0 && row < rows &&
           col >= 0 && col < cols &&
           !seatMatrix[row][col];
}

// Book a seat
void Flight::bookSeat(int row, int col) {
    if (isSeatAvailable(row, col)) {
        seatMatrix[row][col] = true;
        bookedSeats++;
    }
}

// Optional helper functions for admin statistics:

int Flight::getTotalRows() const {
    return rows;
}

int Flight::getTotalCols() const {
    return cols;
}

int Flight::getBookedSeats() const {
    return bookedSeats;
}

int Flight::getTotalSeats() const {
    return totalSeats;
}

int Flight::getRevenue() const {
    return bookedSeats * baseFare;
}

double Flight::getOccupancyRate() const {
    return totalSeats > 0 ? (100.0 * bookedSeats / totalSeats) : 0.0;
}
