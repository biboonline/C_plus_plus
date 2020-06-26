#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  float up_time_process = LinuxParser::UpTime(Pid());
  //float up_time_cpu = LinuxParser::UpTime();
  float totalTime = LinuxParser::ActiveJiffies(Pid());

  cpuUtil =
      ((totalTime / sysconf(_SC_CLK_TCK) / ( up_time_process)));
  return cpuUtil;
}

// Cpu Utilization setter function
void Process::setCpuUtil() { cpuUtil = CpuUtilization(); }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return (Process::cpuUtil < a.cpuUtil);
}