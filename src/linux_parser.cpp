#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

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

// Done: Read and return the system memory utilization 
// Memory Utilization  = TotalMemory - FreeMemory 
// Fucntion returns ratio of memory utilized and Total memory
float LinuxParser::MemoryUtilization() { 

  string line;
  string key;
  string value;
  long MemFree;
  long MemTotal;
  bool MemFreeFound{false}; 
  bool MemTotalFound{false};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          MemTotal = std::stol(value);
          MemTotalFound = true;
        }
        if (key == "MemFree") {
          MemFree = std::stol(value);
          MemFreeFound = true;
        }

        if (MemFreeFound && MemTotalFound) {
          return ((float)MemTotal - (float)MemFree)/(float)MemTotal;
        }

      }
    }
  }
  return  0.0;
  }

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string upTime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return std::stol(upTime); }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string cpu;
  string line;
  string value;
  vector<string> cpuUtilization{};

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu;  // Remove first string
    while (linestream >> value) {
      cpuUtilization.emplace_back(value);
    }
  }
  
  return cpuUtilization;}

// Read and return the total number of jiffies for the system
long LinuxParser::Jiffies() { 

  vector<string> cpuUtilization = LinuxParser::CpuUtilization();

  long total_jiffies{0};

  for (auto data:cpuUtilization) {
      total_jiffies += std::stol(data);
  }

  // remove guest time since its already accounted in user and nice
  total_jiffies = total_jiffies - (std::stol(cpuUtilization[CPUStates::kGuest_]) + std::stol(cpuUtilization[CPUStates::kGuestNice_]));
 
  return total_jiffies; }


// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  
 
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies(); }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 

  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  long idle_jiffies{0};

  idle_jiffies = (std::stol(cpuUtilization[CPUStates::kIdle_]) + std::stol(cpuUtilization[CPUStates::kIOwait_]));

  return idle_jiffies; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);}

//  Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }


// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
