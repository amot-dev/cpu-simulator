#ifndef PIPELINE_H
#define PIPELINE_H

#include <bitset>
#include "IQueue.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ReorderBuffer.h"

class Pipeline{
public:
	Pipeline();
	~Pipeline();

	void fetch();
	//void execute();
	//void commit();

	bool takeInput(std::string);	//loads the program to instructions memory if valid

	bool stillRunning();			//checks if there are still instructions to execute
private:
	uint programCounter = 0;

	Memory Mem;
	RegisterFile RegFile;
	IQueue IQ;
	ReorderBuffer ROB;
};

#endif