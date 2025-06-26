#ifndef ADMIN_H
#define ADMIN_H

#include "BookingSystem.h"
#include <string>

class Admin {
public:
    void showMenu(BookingSystem &system);

    void viewAllBookings(const BookingSystem &system);
    void viewFlightStats(const BookingSystem &system);
    void viewOccupancyAndRevenue(const BookingSystem &system);
};

#endif
