#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int> pids = LinuxParser::Pids();

    // remove duplicate pids
    //pids.erase(std::unique(pids.begin(), pids.end()), pids.end());

    set<int> unique_pids;
    for (Process& process : processes_) {
        unique_pids.insert(process.Pid());
    }

    for (int pid : pids) {
        if (unique_pids.find(pid) == unique_pids.end()) {
            processes_.emplace_back(pid);
        }
    }

    for (auto& process : processes_) {
        long active_jiffies = LinuxParser::ActiveJiffies(process.Pid());
        long jiffies = LinuxParser::Jiffies();
        process.SetCpuUtilization(active_jiffies, jiffies);
    }

    // sort by cpu usage in desc order
    std::sort(processes_.begin(), processes_.end(), [](Process a, Process b) {
        return a > b;
    });

    return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }