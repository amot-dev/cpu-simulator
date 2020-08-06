#include "ReorderBuffer.h"

ReorderBuffer::ReorderBuffer(){};
ReorderBuffer::~ReorderBuffer(){};

void ReorderBuffer::load(bool valid, unsigned instruction){
	validities.push(valid);
	instructions.push(instruction);
	if (ROB_ID.size()) ROB_ID.push(ROB_ID.back() + 1);	//if this is not the first entry, set the ROB_ID to 1 greater than the previous ROB_ID
	else ROB_ID.push(0);								//if it is the first entry, set the ROB_ID to 0
};

void ReorderBuffer::unload(){
	validities.pop();					//pop oldest value from each queue
	instructions.pop();
	ROB_ID.pop();
};

bool ReorderBuffer::empty(){
	if (ROB_ID.size()) return false;	//since all queues are always pushed at the same time, only need to check one
	else return true;
};

short ReorderBuffer::getLastROB_ID(){return ROB_ID.back();};