#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

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
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
   
    std::fstream file("/proc/meminfo" , std::ios::in);

    if(!file.is_open()){
        return 0.0;
    }

    std::string line;
    std::string key;
    float value,Mem_Total,Mem_Free;
    while(getline(file , line)){
        replace(line.begin() , line.end() , ':' , ' ');
        std::stringstream str_in(line);
        str_in >> key >> value;
        
        if(key == "MemTotal"){
            Mem_Total = value;
        }

        else if(key == "MemFree"){
            Mem_Free = value;
            break;
        }
    }

    file.close();
    return (Mem_Total - Mem_Free)/Mem_Total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 

  std::fstream file("/proc/uptime" , std::ios::in);

  if(!file.is_open()){
      return 0;
  }

  std::string uptime;
  file>>uptime;

  file.close();

  long uptime_Total = stol(uptime);

  return uptime_Total; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return (ActiveJiffies() + IdleJiffies()); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 

  std::fstream file(kProcDirectory + to_string(pid) + kStatFilename , std::ios::in);
  std::string line;
  std::vector<std::string> values;
  std::string value;
  long utime{0};            //cpu time spent in user mode (in clock ticks)
  long stime{0};            //cpu time spent in kernel model (int clock ticks)
  
  if (!file.is_open()) {
    return 0;
  }
  if(getline(file, line)) {
    std::istringstream line_ss(line);
    while (line_ss) {
      line_ss >> value;
      values.emplace_back(value);
    }
  }
  utime = stol(values[13]);
  stime = stol(values[14]);
  
  return (utime + stime);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    
    std::fstream file("/proc/stat" , std::ios::in);

    std::string line;
    getline(file , line);

    file.close();
    std::stringstream ss(line);
    std::string token;

    std::vector<std::string>values;

    while(ss >> token){
        values.emplace_back(token);
    }

    long totalActiveJiffies = 0;


    totalActiveJiffies = 
    stol(values[1]) + stol(values[2]) + stol(values[3]) +
    stol(values[6]) + stol(values[7]) + stol(values[8]);

    return totalActiveJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
    std::fstream file("/proc/stat" , std::ios::in);

    if(!file.is_open()){
      return 0;
    }
    std::string line;
    getline(file , line);

    file.close();
    std::stringstream ss(line);
    std::string token;

    std::vector<std::string>values;

    while(ss >> token){
        values.emplace_back(token);
    }

    return (stol(values[4]) + stol(values[5]));
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  
    std::fstream file("/proc/stat" , std::ios::in);

    if(!file.is_open()){
        return 0;
    }

    std::string line , key;
    int total_processes;
    while(getline(file , line)){
        std::stringstream ss(line);
        ss >> key >> total_processes;
        if(key == "processes"){
            break;
        }
    }
    file.close();
    return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 

    std::fstream file("/proc/stat" , std::ios::in);

    if(!file.is_open()){
        return 0;
    }

    string line , key;
    int running_processes;

    while(getline(file , line)){

        std::stringstream ss(line);
        ss >> key >> running_processes;
        if(key == "procs_running"){
            break;
        }
    }
    file.close();
    return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string path = LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename;
  std::fstream file(path , std::ios::in);
  if(!file.is_open()){
      return "Not defined\n";
  }
  std::string command;
  file>>command;
  return command; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  
    std::fstream file(kProcDirectory + to_string(pid) + kStatusFilename , std::ios::in);

    if(!file.is_open()){
      return string();
    }

    std::string line,key,value,unit,result;
    
    while(getline(file , line)){
        replace(line.begin() , line.end() , ':' , ' ');
        std::stringstream ss(line);
        ss>>key>>value>>unit;
        if(key == "VmRSS"){
            result = value + " " + unit;
            break;
        }
    }

    file.close();
    return result; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    
    std::fstream file(kPasswordPath , std::ios::in);

    if(!file.is_open()){
        return string();
    }

    std::string line;
    getline(file , line);
    file.close();

    replace(line.begin() , line.end() , ':' , ' ');
    std::stringstream ss(line);
    std::string user;
    ss>>user;
    return user; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {

  long timeSinceBoot = UpTime();

  std::fstream file(kProcDirectory + to_string(pid) + kStatFilename , std::ios::in);

  if(!file.is_open()){
      return 0;
  }

  int i = 0;
  std::string line;
  
  getline(file , line);
  file.close();

  std::stringstream ss(line);
  long startTime;

  std::string token;

  while(ss >> token){

      if(i == 21){
        startTime = stol(token);
      }
      i++;
  }

  startTime = startTime / (sysconf(_SC_CLK_TCK));

  return (timeSinceBoot - startTime); 
}
