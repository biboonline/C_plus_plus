#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

bool System::compare(Process& p1, Process& p2) { return (p1 < p2); }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  processes_.clear();

  for (int pid : pids) {
    processes_.emplace_back(Process(pid));
  }
  for (unsigned int i = 0; i < sizeof(pids); i++) {
    processes_[i].setCpuUtil();
  }
  std::sort(processes_.begin(), processes_.end(), compare);
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }