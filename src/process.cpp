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

int Process::Pid() { return pid_; }

<<<<<<< HEAD
float Process::CpuUtilization() const {
    return cpu_;
}

void Process::SetCpuUtilization(long a, long b) {
    cpu_ = (float)a / b;
}

string Process::Command() {
    int pid = Pid();
    return LinuxParser::Command(pid);
}

string Process::Ram() {
    int pid = Pid();
    return LinuxParser::Ram(pid);
}

string Process::User() {
    int pid = Pid();
    return LinuxParser::User(pid);
}

long int Process::UpTime() {
    int pid = Pid();
    return LinuxParser::UpTime(pid);
}

bool Process::operator<(Process const& a) const {
=======
float Process::CpuUtilization() { return cpu_;}

void Process::SetCpuUtilization(long active, long all) {
    cpu_ = (float)active / all;
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator>(Process& a) {
>>>>>>> round-two
    return CpuUtilization() > a.CpuUtilization();
}