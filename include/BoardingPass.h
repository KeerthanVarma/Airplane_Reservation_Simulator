// BoardingPass.h
#pragma once
#include <string>
#include "Passenger.h"
#include "Flight.h"

class BoardingPass {
public:
    static void generate(const Passenger& p, const Flight& f, int row, int col);
};
