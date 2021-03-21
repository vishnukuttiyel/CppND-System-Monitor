#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // See src/processor.cpp

  //  Declare any necessary private members
 private:
 float prevTotalJiffies{0};
 float prevActiveJiffies{0};
 float prevCpuPercentage{0};
};

#endif