#ifndef PIPELINE_H
#define PIPELINE_H

#include <bitset>
#include "Execute.h"
#include "IQueue.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ReorderBuffer.h"

class Pipeline{
public:
	Pipeline();
	~Pipeline();

	void fetch();
	void execute();
	//void commit();

	bool takeInput(std::string);	//loads the program to instructions memory if valid

	bool stillRunning();			//checks if there are still instructions to execute
private:
	uint programCounter = 0;		//the current instruction being loaded by the cpu
	bool noFetch = false;			//whether or not the next instruction should be fetched

	Memory Mem;
	RegisterFile RegFile;
	IQueue IQ;
	ReorderBuffer ROB;
};

#endif