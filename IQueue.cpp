#include "IQueue.h"

IQueue::IQueue(){};
IQueue::~IQueue(){};

void IQueue::load(uint opCode, uint dest, bool valid1, uint src1, bool valid2, uint src2){
	operation.push(opCode);								//push all values given to the various queues
	destination.push((int)dest);
	validA.push(valid1);
	sourceA.push((int)src1);							//conversions to int are safe because src1 and src2 never exceed 16 bits
	validB.push(valid2);
	sourceB.push((int)src2);
	if (ROB_ID.size()) ROB_ID.push(ROB_ID.back() + 1);	//if this is not the first entry, set the ROB_ID to 1 greater than the previous ROB_ID
	else ROB_ID.push(0);								//if it is the first entry, set the ROB_ID to 0
};

void IQueue::unload(){
	operation.pop();		//pop oldest value from each queue
	destination.pop();
	validA.pop();
	sourceA.pop();
	validB.pop();
	sourceB.pop();
	ROB_ID.pop();
};