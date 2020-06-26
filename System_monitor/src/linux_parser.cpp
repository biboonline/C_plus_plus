
#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>  // for sleep

#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
  filestream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

//  Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string s, MemTotal, MemFree;
  float used;
  string line, key;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> s;
      if (key == "MemTotal:") {
        MemTotal = s;
      }
      if (key == "MemFree:") {
        MemFree = s;
      }
    }
    used = (std::stof(MemTotal) - std::stof(MemFree)) / std::stof(MemTotal);
  }
  stream.close();
  return used;
}

//  Read and return the system uptime
long LinuxParser::UpTime() {
  string t;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> t;
  }
  stream.close();
  return std::stol(t);
}

//  Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

//  Read and return the number of active jiffies for a PID
float LinuxParser::ActiveJiffies(int pid) {
  string line;
  string key;
  float value = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    // get the 14th 15th 16th 17th values
    for (int i = 1; i <= 17; i++) {
      linestream >> key;
      if (i == 14 || i == 15 || i == 16 || i == 17) value += std::stof(key);
    }
  }
  // filestream.close();
  return value;
}

//  Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

//  Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

//  Read and return CPU utilization
float LinuxParser::CpuUtilization() {
  string user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  float PrevIdle, PrevTotal;

  // read cpu values
  string s, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> s >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
  }
  stream.close();

  PrevIdle = std::stof(idle) + std::stof(iowait);
  PrevTotal = PrevIdle + std::stof(user) + std::stof(nice) + std::stof(system) +
              std::stof(irq) + std::stof(softirq) + std::stof(steal) +
              std::stof(guest) + std::stof(guest_nice);
  return (PrevTotal - PrevIdle) / PrevTotal;
}

//  Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string np = "000";  // No of processes
  string line, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> np;
      if (key == "processes") {
        linestream >> np;
        return std::stoi(np);
      }
    }
  }
  stream.close();
  return std::stoi(np);
}

//  Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string np = "000";  // No of processes
  string line, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> np;
      if (key == "procs_running") {
        linestream >> np;
        return std::stoi(np);
      }
    }
  }
  stream.close();
  return std::stoi(np);
}

//  Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd = "-1";
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> cmd;
    }
    return cmd.substr(1, 40);
  }
  stream.close();
  return cmd.substr(1, 40); // return only first 40 chars from teh command
}

//  Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string Ram = "-1";
  string line, key;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> Ram;
      // if (key == "VmData:") {  // for the actual Ram
      if (key == "VmSize:") {  // Virtual memory size
        // return std::to_string(std::stoi(Ram) / 1024);
        Ram.erase(Ram.end() - 3, Ram.end());  // remve last 3 chars
        return Ram;
      }
    }
  }
  stream.close();
  // return std::to_string(std::stoi(Ram) / 1024);
  Ram.erase(Ram.end() - 3, Ram.end());  // remve last 3 chars
  return Ram;
}

//  Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string Uid = "-1";
  string line, key;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> Uid;
      if (key == "Uid:") {
        return Uid;
      }
    }
  }
  stream.close();
  return Uid;
}

//  Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string User = "-1";
  string Uid = LinuxParser::Uid(pid);
  string line, key, x;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> User >> x >> key;
      if (key == Uid) {
        return User;
      }
    }
  }
  stream.close();
  return User;
}

//  Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string up_time = "-1";
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int i = 0; i < 22; i++) {
        linestream >> up_time;
        // if (i == 21) return (std::stol(up_time) / sysconf(_SC_CLK_TCK));
      }
    }
  }
  stream.close();
  return (UpTime() - (std::stol(up_time) / sysconf(_SC_CLK_TCK)));
}