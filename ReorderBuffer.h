#ifndef REORDERBUFFER_H
#define REORDERBUFFER_H

#include <list>

struct ReorderBufferSet{
	bool valid;
	unsigned instruction;
	short ROB_ID;
};

class ReorderBuffer{
public:
	ReorderBuffer();
	~ReorderBuffer();

	void load(unsigned instruction);				//pushes a new instruction to the ROB
	void unloadOldestIfValid();						//pops the oldest set of validated instructions from the ROB
	bool empty();									//returns true if the ROB is empty

	short getLastROB_ID();							//returns the last ROB_ID inputted into the queue
	void setValidity(short ID);						//changes validity to true for the specified ID
	
private:
	std::list<ReorderBufferSet> ROB_SET;
};

#endif