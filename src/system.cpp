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

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

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

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
