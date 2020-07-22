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

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    // try to maintain a list of process opbjects instead of simply creating new ones on every interation
    // this is a bit more complicated, but enables the usage of cached values within Process objects.
    // one might argue that simplicity should be preferred to caching... but this here is just a design
    // decision for training purposes

    std::vector<Process> newProcessList{};

    // re-use Process objects from the current list as long as they're still alive
    // (still alive == part of the PID-list from LinuxParser)
    // only add missing PIDs by creating new Process objects
    for (int pid : LinuxParser::Pids()) {
        bool containsPID = false;
        for (Process p_ : processes_) {
          if (p_.Pid() == pid) {
              newProcessList.push_back(p_);
              containsPID = true;
              break;
          }
        }
        if (containsPID == false) {
          Process p(pid);
          newProcessList.push_back(p);
        }
    }

    // sort newProcessList
    std::sort(newProcessList.begin(), newProcessList.end());

    // remove old list and return the new one instead
    processes_.swap(newProcessList);
    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
    if (kernel_ == "") kernel_ = LinuxParser::Kernel();

    return kernel_;
}

// DONE: Return the system's memory utilization
float System::MemoryUtilization() {
    if (totalMemory_ == 0) totalMemory_ = LinuxParser::MemoryUtilization("MemTotal");
    float memFree = LinuxParser::MemoryUtilization("MemFree");

    return (totalMemory_ - memFree) / totalMemory_;
}

// DONE: Return the operating system name
std::string System::OperatingSystem() {
    if (operatingSystem_ == "") operatingSystem_ = LinuxParser::OperatingSystem();

    return operatingSystem_;
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}

// DONE: Return the total number of processes on the system
int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}

int System::CurrentProcesses()
{
    return processes_.size();
}

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
