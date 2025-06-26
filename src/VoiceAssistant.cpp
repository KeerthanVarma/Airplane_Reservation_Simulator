#include "../include/VoiceAssistant.h"
#include <cstdlib>
#include <string>

void speak(const std::string &text) {
#ifdef _WIN32
    std::string safeText = text;
    for (char &c : safeText) {
        if (c == '"') c = ' ';  // remove double-quotes to avoid PowerShell parsing issues
    }

    std::string command = "PowerShell -Command \"Add-Type -AssemblyName System.Speech; "
                          "$speak = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
                          "$speak.Rate = -2; $speak.Speak(\\\"" + safeText + "\\\")\"";
#elif __APPLE__
    std::string command = "say \"" + text + "\"";
#else
    std::string command = "espeak -s 130 \"" + text + "\"";
#endif
    system(command.c_str());
}
