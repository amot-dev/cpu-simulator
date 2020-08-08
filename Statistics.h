#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream>
#include <vector>
#include <unordered_map>

class Statistics{
public:
	Statistics();
	Statistics(int size);
	~Statistics();

	int getCycle();							//returns the current clock cycle
	void incrementCycles();					//increments the clock
	void incrementLatency(short ROB_ID);	//increments the latency associated with the given ROB_ID by 1
	void incrementLatestThroughput();		//increment latestThroughput by one (executed for each instruction passed through execute() in a cycle)
	void pushLatestThroughput();			//pushes latestThroughput to a vector for storage (executed once per cycle)

private:
	int totalCycles = 0;					//keeps track of the clock cycles
	int latestThroughput = 0;
	std::unordered_map<short,int> latencies;
	std::vector<int> throughputs;
};

#endif