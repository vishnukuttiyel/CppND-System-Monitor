#include <string>
#include <sstream>
#include <iomanip> 

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hh, mm, ss;
    hh = (int) seconds/3600;
    mm = (int) (seconds/60 - hh*60);
    ss = (int) (seconds)%60;

    std::stringstream hhmmss;

    hhmmss<<std::setfill('0') << std::setw(2) << hh << ':'<<std::setfill('0') << std::setw(2) << mm << ':' <<std::setfill('0') << std::setw(2) << ss;
    return hhmmss.str(); }