#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

    long active_jiffies = LinuxParser::ActiveJiffies();
    long total_jiffies = LinuxParser::Jiffies();

    float perc = float(active_jiffies)/float(total_jiffies);

    return perc; 
}