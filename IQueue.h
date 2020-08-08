#ifndef IQUEUE_H
#define IQUEUE_H

#include <string>
#include <queue>

class IQueue{
public:
	IQueue();
	~IQueue();

	void load(uint opCode, uint dest, uint src1, uint src2, int immediate, short ID);	//pushes a new set of values to the instruction queue
	void unloadOldest();																//pops the oldest set of values from the instruction queue
	int size();																			//returns the amount of instructions in the IQ

	uint getOperation();	//accessor functions
	int getDestination();
	int getSourceA();
	int getSourceB();
	int getImmediateVal();
	short getROB_ID();

private:
	std::queue<uint> operation;		//stores the operation codes
	std::queue<int> destination;	//stores the destination of the operation
	std::queue<int> sourceA;		//the address of the register containing A
	std::queue<int> sourceB;		//the address of the register containing A
	std::queue<int> immediateVal;	//the immediate value from the user
	std::queue<short> ROB_ID;		//the ROB_ID of the operation
};

#endif