#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  std::string key, line;
  int value, MemTotal, MemFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
            if (key == "MemTotal") {
                MemTotal = value;
            } else if (key == "MemFree") {
                MemFree = value;
            }
        }
    }
  }
  int totalUsed = MemTotal - MemFree;
  float total = (float)totalUsed / MemTotal;
  return total;
}

long LinuxParser::UpTime() {
  long systemUptime, idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> systemUptime >> idleTime;
  }
  return systemUptime + idleTime;
}

long LinuxParser::Jiffies() {
  return UpTime() * sysconf(_SC_CLK_TCK);
}

long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  vector<string> values;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  if (values.size() > 21) {
    long a = stol(values[13]);
    long b = stol(values[14]);
    long c = stol(values[15]);
    long d = stol(values[16]);
    return a + b + c + d; 
  }
  return 0;
}

long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = CpuUtilization();
  return (std::stol(jiffies[0]) + std::stol(jiffies[1]) +
    std::stol(jiffies[2]) + std::stol(jiffies[5]) +
    std::stol(jiffies[6]) + std::stol(jiffies[7]) +
    std::stol(jiffies[8]) + std::stol(jiffies[9]));
}

long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  return (std::stol(jiffies[3]) + std::stol(jiffies[4]));
}

vector<string> LinuxParser::CpuUtilization() {
  string line, key, value;
  vector<string> values;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          while (linestream >> value) {
            values.push_back(value);
          }
        }
      }
    }
  }
  return values;
}

int LinuxParser::TotalProcesses() {
  std::string key, line;
  int value = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
        if (line.rfind("processes", 0) == 0) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          return value;
        }
      }
    }
  }
  return value;
}

int LinuxParser::RunningProcesses() {
  std::string key, line;
  int value = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
        if (line.rfind("procs_running", 0) == 0) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    if (line == "") {
      return "---";
    } else {
      return line;
    }
  }
  return "Unknown";
}

string LinuxParser::Ram(int pid) {
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (stream >> value) {
      if (value == "VmSize:") {
        if (stream >> value) {
          return std::to_string(stoi(value) / 1024);
        }
      }
    }
  }
  return "---";
}

string LinuxParser::Uid(int pid) {
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (stream >> value) {
      if (value == "Uid:") {
        if (stream >> value) {
          return value;
        }
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string line;
  string uid = "x:" + Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      auto position = line.find(uid);
      if (position != string::npos) {
        return line.substr(0, position - 1);
      }
    }
  }
  return "Unknown";
}

long LinuxParser::UpTime(int pid) {
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    for (int i = 0; stream >> value; ++i) {
      if (i == 13) {
        long time = stol(value);
        time = time / sysconf(_SC_CLK_TCK);
        return time;
      }
    }
  }
  return 0.0;
}