//#define DEBUG //uncomment to enable debug mode (also in Execute.cpp and Pipeline.cpp)

#include "ReorderBuffer.h"

ReorderBuffer::ReorderBuffer(){};
ReorderBuffer::~ReorderBuffer(){};

void ReorderBuffer::load(unsigned instruction){
	ReorderBufferSet loader;										//create temporary struct to assign all values
	loader.valid = false;											//instruction has not been executed if it is being loaded
	loader.instruction = instruction;
	loader.ROB_ID = ROB_IDCounter++;								//set the ROB_ID to the ROB_ID counter, then increment counter
	ROB_SET.push_back(loader);										//push back temporary struct to ROB_SET
};

void ReorderBuffer::unloadOldestIfValid(){
	if (ROB_SET.front().valid){
		#ifdef DEBUG
			std::cout << "\033[34mComitting\033[0m ROB_ID[" << ROB_SET.front().ROB_ID << "]\n";
		#endif
		ROB_SET.pop_front();					//if the oldest instruction is validated, pop it
	};
};

int ReorderBuffer::size(){return ROB_SET.size();};

short ReorderBuffer::getLastROB_ID(){return ROB_SET.back().ROB_ID;};
std::list<short> ReorderBuffer::getROB_IDs(){
	std::list<short> temp;
	for (auto it = ROB_SET.begin(); it != ROB_SET.end(); it++) temp.push_back(it->ROB_ID);
	return temp;
};

void ReorderBuffer::setValidity(short ID){for (auto it = ROB_SET.begin(); it != ROB_SET.end(); it++) if (it->ROB_ID == ID) it->valid = true;};