#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

#define HOUR 3600
#define MIN 60

using std::string;

string Format::ElapsedTime(long seconds) { 
    int hh = seconds / HOUR;
    seconds %= HOUR;
    int mm = seconds / MIN;
    seconds %= MIN;
    int ss = seconds;

    std::stringstream result;
    result << std::setfill('0') << std::setw(2) << hh << ":"
        << std::setfill('0') << std::setw(2) << mm << ":"
        <<  std::setfill('0') << std::setw(2) << ss;
    return result.str();
}