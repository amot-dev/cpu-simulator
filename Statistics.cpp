#include "Statistics.h"

Statistics::Statistics(){};
Statistics::Statistics(int size){
	latencies.reserve(size);
	throughputs.reserve(size);
};

Statistics::~Statistics(){
	double averageLatency = 0;
	double averageThroughput = 0;
	for (std::vector<int>::iterator it = latencies.begin(); it != latencies.end(); it++) averageLatency += *it;
	for (std::vector<int>::iterator it = throughputs.begin(); it != throughputs.end(); it++) averageThroughput += *it;
	std::cout << "Average latency per instruction: " << averageLatency << "\n";
	std::cout << "Average instruction throughput per cycle: " << averageThroughput << "\n";
};

void Statistics::incrementLatestLatency(){latestLatency++;};
void Statistics::incrementLatestThroughput(){latestThroughput++;};
void Statistics::pushLatest(){
	latencies.push_back(latestLatency);
	throughputs.push_back(latestThroughput);
};