#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
    return cpu_;
}

void Process::SetCpuUtilization(long a, long b) {
    cpu_ = (float)a / b;
}

// TODO: Return the command that generated this process
string Process::Command() {
    int pid = Pid();
    return LinuxParser::Command(pid);
}

// DONE: Return this process's memory utilization
string Process::Ram() {
    int pid = Pid();
    return LinuxParser::Ram(pid);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    int pid = Pid();
    return LinuxParser::User(pid);
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
    int pid = Pid();
    return LinuxParser::UpTime(pid);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }