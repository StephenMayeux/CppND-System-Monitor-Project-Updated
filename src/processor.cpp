#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    float result = 0;
    long active_jiffies = LinuxParser::ActiveJiffies();
    long idle_jiffies = LinuxParser::IdleJiffies();
    long total_jiffies = active_jiffies + idle_jiffies;
    result = (float)active_jiffies / total_jiffies;
    return result;
}