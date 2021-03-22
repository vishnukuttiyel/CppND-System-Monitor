#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // See src/processor.cpp

  //  Declare any necessary private members
 private:
 float prevTotalJiffies_{0};
 float prevActiveJiffies_{0};
 float prevCpuPercentage_{0};
};

#endif