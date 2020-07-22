#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization(std::string param) {
    string line;
    string key;
    float value = 0;
    string unit;
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::remove(line.begin(), line.end(), ':');
        std::istringstream linestream(line);
        while (linestream >> key >> value >> unit) {
          if (key == param) {
            return value;
          }
        }
      }
    }
    return value;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
    long uptime{0}, totalIdleTime{0};
    string line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> uptime >> totalIdleTime;
    }
    return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
std::vector<float> LinuxParser::CpuUtilization() {
    string key;
    float user, nice, system, idle;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        if (linestream >> key >> user >> nice >> system >> idle) {
          if (key == "cpu") {
              return {user, nice, system, idle};
          }
        }
      }
    }

    return {};
}

int LinuxParser::ProcStat(std::string param) {
    string key;
    int value;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> key >> value;
        if (key == param) return value;
      }
    }

    return 0;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    return ProcStat("processes");
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    return ProcStat("procs_running");
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string line;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()) {
      if (std::getline(filestream, line))
          return line;
    }

    return string();
}

int LinuxParser::ProcessStatus(int pid, std::string param) {
    string line;
    string key;
    int value = 0;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::remove(line.begin(), line.end(), ':');
        std::istringstream linestream(line);
        if (linestream >> key >> value) {
          if (key == param) {
            return value;
          }
        }
      }
    }

    return 0;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
int LinuxParser::Ram(int pid) {
    return ProcessStatus(pid, "VmSize");
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
int LinuxParser::Uid(int pid) {
    return ProcessStatus(pid, "Uid");
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string line;
    int key;
    string X, user;
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ' ', '_');
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        if (linestream >> user >> X >> key) {
          if (key == pid) {
            std::replace(user.begin(), user.end(), '_', ' ');
            return user;
          }
        }
      }
    }

    return "";
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    string line;
    long value = 0;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
      if (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        // skip the first 3 entries (consisting of mixed types 'int' and 'string'
        linestream.ignore(256,' ');   // ignore until space
        linestream.ignore(256,' ');   // ignore until space
        linestream.ignore(256,' ');   // ignore until space
        int counter = 4;
;        while (linestream >> value) {
          if (counter == 22)
              return value;

          counter++;
        }
      }
    }

    return 0;
}
