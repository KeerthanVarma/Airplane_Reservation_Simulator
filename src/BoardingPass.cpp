#include "../include/BoardingPass.h"
#include "../include/VoiceAssistant.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "../include/BoardingPass.h"
#include "../include/VoiceAssistant.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

void BoardingPass::generate(const Passenger &p, const Flight &f, int row, int col) {
    std::string seat = std::to_string(row + 1) + static_cast<char>('A' + col);
    std::string filename = "BoardingPass_" + p.passportNumber + "_" + f.flightID + ".txt";

    std::ostringstream out;
    out << "====================================================\n";
    out << "                 DOMESTIC AIRLINES                  \n";
    out << "====================================================\n";
    out << "Passenger Name     : " << p.name << "\n";
    out << "Aadhaar Number     : " << p.passportNumber << "\n";
    out << "Flight ID          : " << f.flightID << "\n";
    out << "From               : " << f.source << "\n";
    out << "To                 : " << f.destination << "\n";
    out << "Date               : " << f.date << "\n";
    out << "Time               : " << f.time << "\n";
    out << "Seat               : " << seat << "\n";
    out << "Gate               : 5B\n";
    out << "Boarding Time      : 45 minutes before departure\n";
    out << "====================================================\n";
    out << "Note: Please carry a valid ID proof. Be at the gate\n";
    out << "at least 45 minutes before departure.\n";
    out << "====================================================\n";

    // Write to file
    std::ofstream fileOut(filename);
    if (!fileOut) {
        std::string errorMsg = "Error generating boarding pass.";
        std::cout << errorMsg << std::endl;
        speak(errorMsg);
        return;
    }
    fileOut << out.str();
    fileOut.close();

    // Print on console
    std::cout << out.str();

    speak("Your boarding pass has been successfully generated.");
}
