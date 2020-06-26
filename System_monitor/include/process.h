#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  // Define the constructor
  Process(int i) : pid(i){};
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  void setCpuUtil();

  // Declare any necessary private members
 private:
  int pid;
  //   std::string uid;
  //   std::string cmd;
  float cpuUtil = 0.0;
  //   std::string ram;
  //   long int up_time;
};

#endif