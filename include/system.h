#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  static long UpTime();                      // DONE: See src/system.cpp

  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // DONE: See src/system.cpp
  float MemoryUtilization();          // DONE: See src/system.cpp
  int TotalProcesses();               // DONE: See src/system.cpp
  int CurrentProcesses();
  int RunningProcesses();             // DONE: See src/system.cpp
  std::string Kernel();               // DONE: See src/system.cpp
  std::string OperatingSystem();      // DONE: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};

  // caching some 'stable' information instead of reparsing the files all the time
  std::string kernel_ = "";
  std::string operatingSystem_ = "";
  float totalMemory_ = 0;
};

#endif
