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
	averageThroughput = averageThroughput/(totalCycles-2);	//get average executions per cycle (-2 because there are never executions in the first or last cycle)
	if (totalCycles != 0){									//destructor is called even if a program is not run, so it must ensure a program has run
		std::cout << "\n\033[1m\033[37mAverage latency per instruction: \033[0m" << averageLatency << "\n";
		std::cout << "\033[1m\033[37mAverage instruction throughput per cycle: \033[0m" << averageThroughput << "\n";
	};
};

int Statistics::getCycle(){return totalCycles;};
void Statistics::incrementCycles(){totalCycles++;};
void Statistics::incrementLatency(short ROB_ID){latencies[ROB_ID] += 1;};
void Statistics::incrementLatestThroughput(){latestThroughput++;};

void Statistics::pushLatestThroughput(){
	throughputs.push_back(latestThroughput);	//store latestThroughput
	latestThroughput = 0;						//reset it to 0 for the next cycle
};