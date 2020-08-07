#include "Statistics.h"

Statistics::Statistics(){};
Statistics::Statistics(int size){
	latencies.reserve(size);
	throughputs.reserve(size);
};

Statistics::~Statistics(){
	double averageLatency = 0;
	double averageThroughput = 0;
	for (auto it = latencies.begin(); it != latencies.end(); it++) averageLatency += it->second;
	for (auto it = throughputs.begin(); it != throughputs.end(); it++) averageThroughput += *it;
	averageLatency = averageLatency/averageThroughput;		//at this point "average" throughput is really just total throughput, which is total instructions
	averageThroughput = averageThroughput/totalCycles;		//get average instruction throughput per cycle
	std::cout << "Average latency per instruction: " << averageLatency << "\n";
	std::cout << "Average instruction throughput per cycle: " << averageThroughput << "\n";
};

void Statistics::incrementCycles(){totalCycles++;};
void Statistics::incrementLatency(short ROB_ID){latencies[ROB_ID] += 1;};
void Statistics::incrementLatestThroughput(){latestThroughput++;};
void Statistics::pushLatestThroughput(){
	throughputs.push_back(latestThroughput);	//store latestThroughput
	latestThroughput = 0;						//reset it to 0 for the next cycle
};