#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();
  // void setPID (int i);
  static bool compare(Process& p1, Process& p2);

  // Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif