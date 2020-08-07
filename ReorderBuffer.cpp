#include "ReorderBuffer.h"

ReorderBuffer::ReorderBuffer(){};
ReorderBuffer::~ReorderBuffer(){};

void ReorderBuffer::load(unsigned instruction){
	ReorderBufferSet loader;										//create temporary struct to assign all values
	loader.valid = false;											//instruction has not been executed if it is being loaded
	loader.instruction = instruction;
	if (ROB_SET.size()) loader.ROB_ID = ROB_SET.back().ROB_ID + 1;	//if this is not the first entry, set the ROB_ID to 1 greater than the previous ROB_ID
	else loader.ROB_ID = 0;											//if it is the first entry, set the ROB_ID to 0
	ROB_SET.push_back(loader);										//push back temporary struct to ROB_SET
};

void ReorderBuffer::unloadOldestIfValid(){
	if (ROB_SET.front().valid) ROB_SET.pop_front();					//if the oldest instruction is validated, pop it
};

bool ReorderBuffer::empty(){
	if (ROB_SET.size()) return false;	//check if ROB is empty and return true if it is
	else return true;
};

short ReorderBuffer::getLastROB_ID(){return ROB_SET.back().ROB_ID;};
std::list<short> ReorderBuffer::getROB_IDs(){
	std::list<short> temp;
	for (auto it = ROB_SET.begin(); it != ROB_SET.end(); it++) temp.push_back(it->ROB_ID);
	return temp;
};

void ReorderBuffer::setValidity(short ID){for (auto it = ROB_SET.begin(); it != ROB_SET.end(); it++) if (it->ROB_ID = ID) it->valid = true;};