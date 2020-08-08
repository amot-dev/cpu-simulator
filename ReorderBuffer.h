#ifndef REORDERBUFFER_H
#define REORDERBUFFER_H

#include <iostream>
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

	void load(unsigned instruction);			//pushes a new instruction to the ROB
	void unloadOldestIfValid();					//pops the oldest set of validated instructions from the ROB
	int size();									//returns size of the ROB

	short getLastROB_ID();						//returns the last ROB_ID inputted into the queue
	std::list<short> getROB_IDs();				//returns all ROB_IDs  (used by stats to see if a set of instructions have finished processing)
	void setValidity(short ID);					//changes validity to true for the specified ID
	
private:
	short ROB_IDCounter = 0;
	std::list<ReorderBufferSet> ROB_SET;
};

#endif