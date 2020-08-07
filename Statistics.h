#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream>
#include <vector>

class Statistics{
public:
	Statistics();
	Statistics(int size);
	~Statistics();

	void incrementLatestLatency();
	void incrementLatestThroughput();
	void pushLatest();

private:
	int latestLatency = 0;
	int latestThroughput = 0;
	std::vector<int> latencies;
	std::vector<int> throughputs;
};

#endif