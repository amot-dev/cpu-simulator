#ifndef PIPELINE_H
#define PIPELINE_H

#include <iostream>
#include <iomanip>
#include <bitset>
#include "Exception.h"
#include "Execute.h"
#include "IQueue.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ReorderBuffer.h"
#include "Statistics.h"

class Pipeline{
public:
	Pipeline();
	~Pipeline();

	bool takeInput(std::string);	//loads the program to instructions memory if valid
	bool stillRunning();			//checks if there are still instructions to execute
	void doClockCycle();			//performs necessarry actions on each clock cycle

	void fetch();					//fetches instructions from memory and decodes them to the IQ and ROB
	void execute();					//executes instructions from the IQ, then removes them
	void commit();					//verifies instructions completed and removes them from ROB
	
private:
	int estimatedInstructionCount = 25;	//the estimated number of instructions; used to reserve vector space
	uint programCounter = 0;			//the current instruction being loaded by the cpu
	bool noFetch = false;				//whether or not the next instruction should be fetched

	Memory Mem;
	RegisterFile RegFile;
	IQueue IQ;
	ReorderBuffer ROB;
	Statistics Stats;
};

#endif