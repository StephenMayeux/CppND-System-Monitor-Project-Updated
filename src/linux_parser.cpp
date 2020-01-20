#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "format.h"

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
<<<<<<< HEAD
  string os, theWordVersion, kernel;
=======
  string os, version, kernel;
>>>>>>> round-two
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
<<<<<<< HEAD
    linestream >> os >> theWordVersion >> kernel;
=======
    linestream >> os >> version >> kernel;
>>>>>>> round-two
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
<<<<<<< HEAD
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
=======
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
>>>>>>> round-two
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
<<<<<<< HEAD
  long a = stol(values[13]);
  long b = stol(values[14]);
  long c = stol(values[15]);
  long d = stol(values[16]);
  return a + b + c + d;
=======
  if (values.size() > 21) {
    long a = stol(values[13]);
    long b = stol(values[14]);
    long c = stol(values[15]);
    long d = stol(values[16]);
    return a + b + c + d; 
  }
  return 0;
>>>>>>> round-two
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

<<<<<<< HEAD
vector<string> LinuxParser::CpuUtilization() { 
=======
vector<string> LinuxParser::CpuUtilization() {
>>>>>>> round-two
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
<<<<<<< HEAD
  return values;  
=======
  return values;
>>>>>>> round-two
}

int LinuxParser::TotalProcesses() {
  std::string key, line;
<<<<<<< HEAD
  int value;
=======
  int value = 0;
>>>>>>> round-two
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
<<<<<<< HEAD
  int value;
=======
  int value = 0;
>>>>>>> round-two
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
<<<<<<< HEAD
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string();
=======
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
>>>>>>> round-two
}

string LinuxParser::Ram(int pid) {
  string value;
<<<<<<< HEAD
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
=======
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
>>>>>>> round-two
  if (stream.is_open()) {
    while (stream >> value) {
      if (value == "VmSize:") {
        if (stream >> value) {
<<<<<<< HEAD
          return std::to_string(stoi(value));
=======
          return std::to_string(stoi(value) / 1024);
>>>>>>> round-two
        }
      }
    }
  }
<<<<<<< HEAD
  return string();
=======
  return "---";
>>>>>>> round-two
}

string LinuxParser::Uid(int pid) {
  string value;
<<<<<<< HEAD
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename);
=======
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
>>>>>>> round-two
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
<<<<<<< HEAD
  std::ifstream stream(LinuxParser::kPasswordPath);
=======
  std::ifstream stream(kPasswordPath);
>>>>>>> round-two
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      auto position = line.find(uid);
      if (position != string::npos) {
        return line.substr(0, position - 1);
      }
    }
  }
<<<<<<< HEAD
  return string();
=======
  return "Unknown";
>>>>>>> round-two
}

long LinuxParser::UpTime(int pid) {
  string value;
<<<<<<< HEAD
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    for (int i = 0; stream >> value; i++) {
      if (i == 13) {
        long time = stol(value);
=======
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    for (int i = 0; stream >> value; ++i) {
      if (i == 13) {
        long time = stol(value);
        time = time / sysconf(_SC_CLK_TCK);
>>>>>>> round-two
        return time;
      }
    }
  }
  return 0.0;
}