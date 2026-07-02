#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
  int pid_ = pid;
  long runtime_ = 0;
  long uptime_ = 0;
  float cpu_utilization_ = 0.0;
  std::string user_ = "Sample User";
  // std::string command_ = "Sample Command";
  command_ = LinuxParser::Command(pid_);
  std::string ram_ = "Sample Ram";
}


// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  
  long active_jiffies = LinuxParser::ActiveJiffies(pid_);
  long total_uptime = LinuxParser::UpTime();
  return float(active_jiffies)/float(total_uptime);

  // return float(LinuxParser::ActiveJiffies(pid_))/float(LinuxParser::UpTime()); 
}

// TODO: Return the command that generated this process
string Process::Command() { 
  return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
  return LinuxParser::Ram(pid_); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(pid_); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime(pid_); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return pid_ < a.pid_; 
}