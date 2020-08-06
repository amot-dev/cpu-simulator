#ifndef IQUEUE_H
#define IQUEUE_H

#include <string>
#include <queue>

class IQueue{
public:
	IQueue();
	~IQueue();

	void load(uint opCode, uint dest, bool valid1, uint src1, bool valid2, uint src2);	//pushes a new set of values to the instruction queue
	void unload();																		//pops the oldest set of values from the instruction queue

private:
	std::queue<uint> operation;			//stores the operation codes
	std::queue<int> destination;		//stores the destination of the operation
	std::queue<bool> validA;			//stores whether or not sourceA is valid
	std::queue<int> sourceA;
	std::queue<bool> validB;
	std::queue<int> sourceB;
	std::queue<short> ROB_ID;
};

#endif