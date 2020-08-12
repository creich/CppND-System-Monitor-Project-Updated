#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include <thread>

#include "process.h"

#include "linux_parser.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
    std::vector<float> cpuStats = LinuxParser::CpuUtilization(pid_);
    if (cpuStats.size() == 5) {
      float utime = cpuStats[0];
      float stime = cpuStats[1];
      float cutime = cpuStats[2];
      float cstime = cpuStats[3];
      float starttime = cpuStats[4];

      float total = utime + stime + cutime + cstime;
      float totalD = total - cpuTotalLast_;

      long clk_tck = sysconf(_SC_CLK_TCK);
      float seconds = LinuxParser::UpTime() - (starttime / clk_tck);
      float secondsD = seconds - cpuSecondsLast_;

      float cpu_usage = 0;
      if (secondsD > 0)
          cpu_usage = 100 * ((totalD / clk_tck) / secondsD);
      unsigned int ncpu = std::thread::hardware_concurrency();
      if (ncpu > 0)
          cpu_usage /= ncpu;

      // saving values for next iteration
      cpuTotalLast_ = total;
      cpuSecondsLast_ = seconds;

      return cpu_usage;
    }

    return 0.0;
}

// DONE: Return the command that generated this process
string Process::Command() {
    if (command_ == "") command_ = LinuxParser::Command(pid_);

    return command_;
}

// DONE: Return this process's memory utilization
string Process::Ram() const {
    return std::to_string(LinuxParser::Ram(pid_) / 1024);
}

// DONE: Return the user (name) that generated this process
string Process::User() {
    if (user_ == "") {
        int uid = LinuxParser::Uid(pid_);
        user_ = LinuxParser::User(uid);
    }

    return user_;
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() const {
    //     System::UpTime   - process started at clockTicks after boot / sysconf(_SC_CLK_TCK)
    return System::UpTime() - LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
}

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    // intentionally turned around to see other processes than init per default
    return std::stoi(this->Ram(), nullptr) > std::stoi(a.Ram(), nullptr);

// possible variants:
//    return pid_ > a.pid_;   // intentionally turned around to see other processes than init per default
//    return pid_ < a.pid_;
//    return this->UpTime() > a.UpTime();
}
