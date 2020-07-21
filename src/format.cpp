#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long totalSeconds) {
    int hours = totalSeconds / 3600;
    int minutes = totalSeconds / 60 - hours * 60;
    int seconds = totalSeconds - minutes * 60 - hours * 3600;

    return string() + std::to_string(hours) + ":" + std::to_string(minutes) + (seconds > 9 ? ":" : ":0") + std::to_string(seconds);
}
