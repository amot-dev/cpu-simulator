#ifndef REORDERBUFFER_H
#define REORDERBUFFER_H

#include <queue>

class ReorderBuffer{
public:
	ReorderBuffer();
	~ReorderBuffer();

	void load(bool valid, unsigned instruction);	//pushes a new set of values to the ROB
	void unload();									//pops the oldest set of values from the ROB
	bool empty();									//returns true if the ROB is empty

	short getLastROB_ID();							//returns the last ROB_ID inputted into the queue
	
private:
	std::queue<bool> validities;
	std::queue<unsigned> instructions;
	std::queue<short> ROB_ID;
};

#endif