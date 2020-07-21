#include "processor.h"

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<float> cpuStats = LinuxParser::CpuUtilization();
    if (cpuStats.size() == 4) {
      float user = cpuStats[0];
      float nice = cpuStats[1];
      float system = cpuStats[2];
      float idle = cpuStats[3];

      float total = user + nice + system + idle;
      float busy = total - idle;

      float totalD = total - totalLast_;
      float busyD = busy - busyLast_;

      // saving values for next iteration
      totalLast_ = total;
      busyLast_ = busy;

      if (total > 0) return busyD / totalD;
    }

    return 0.0;
}
