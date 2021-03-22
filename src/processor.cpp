#include "processor.h"
# include "linux_parser.h"

//  Return the aggregate CPU utilization
float Processor::Utilization() { 

    float cpuPercentage;

    // calculate cpu usage by dividing delta active jiffies with delta total jiffies
    float currentTotalJiffies = (float)LinuxParser::Jiffies();
    float currentActiveJiffies = (float)LinuxParser::ActiveJiffies();


    float deltaTotalJiffies = currentTotalJiffies-prevTotalJiffies_;
    float deltaActiveJiffies = (currentActiveJiffies - prevActiveJiffies_);

    if ((deltaTotalJiffies >0) && (deltaActiveJiffies >0)) {
        cpuPercentage = deltaActiveJiffies / deltaTotalJiffies;
    }
    else {
        cpuPercentage = prevCpuPercentage_;
    }
    

    prevTotalJiffies_ = currentTotalJiffies;
    prevActiveJiffies_ = currentActiveJiffies; 
    prevCpuPercentage_ = cpuPercentage;
    return cpuPercentage;}