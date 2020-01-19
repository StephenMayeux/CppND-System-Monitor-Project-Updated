#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();

    for (int pid : pids) {
        Process p (pid);
        processes_.push_back(p);
    }

    for (auto& process : processes_) {
        long active_jiffies = LinuxParser::ActiveJiffies(process.Pid());
        long jiffies = LinuxParser::Jiffies();
        process.SetCpuUtilization(active_jiffies, jiffies);
    }

    return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }