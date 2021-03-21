#include "processor.h"
# include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

    float cpuPercentage;

    // calculate cpu usage by dividing delta active jiffies with delta total jiffies
    float currentTotalJiffies = (float)LinuxParser::Jiffies();
    float currentActiveJiffies = (float)LinuxParser::ActiveJiffies();


    float deltaTotalJiffies = currentTotalJiffies-prevTotalJiffies;
    float deltaActiveJiffies = (currentActiveJiffies - prevActiveJiffies);

    if ((deltaTotalJiffies >0) && (deltaActiveJiffies >0)) {
        cpuPercentage = deltaActiveJiffies / deltaTotalJiffies;
    }
    else {
        cpuPercentage = prevCpuPercentage;
    }
    

    prevTotalJiffies = currentTotalJiffies;
    prevActiveJiffies = currentActiveJiffies; 
    prevCpuPercentage = cpuPercentage;
    return cpuPercentage;}