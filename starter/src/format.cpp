#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) { 
    
    long hours_L = seconds/3600;
    long minutes_L = (seconds%3600)/60;
    long seconds_L = seconds%60;

    std::string hours = std::to_string(hours_L);
    if(hours_L < 10){
        hours = "0" + hours;
    }

    std::string minutes = std::to_string(minutes_L);
    if(minutes_L < 10){
        minutes = "0" + minutes;
    }

    std::string seconds_string = std::to_string(seconds_L);
    if(seconds_L < 10){
        seconds_string = "0" + seconds_string;
    }
    return (hours + ":" + minutes + ":" + seconds_string);
}