#ifndef IQUEUE_H
#define IQUEUE_H

#include <string>
#include <queue>

class IQueue{
public:
	IQueue();
	~IQueue();

	void load(short, int, bool, int, bool, int);	//pushes a new set of values to the instruction queue
	void unload();									//pops the oldest set of values from the instruction queue

private:
	std::queue<short> operation;
	std::queue<int> destination;
	std::queue<bool> validA;
	std::queue<int> sourceA;
	std::queue<bool> validB;
	std::queue<int> sourceB;
	std::queue<short> ROB_ID;
};

#endif