#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
<<<<<<< HEAD
    float result = 0;
=======
    float result = 0.0;
>>>>>>> round-two
    long active_jiffies = LinuxParser::ActiveJiffies();
    long idle_jiffies = LinuxParser::IdleJiffies();
    long total_jiffies = active_jiffies + idle_jiffies;
    result = (float)active_jiffies / total_jiffies;
    return result;
}