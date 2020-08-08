#include "IQueue.h"

IQueue::IQueue(){};
IQueue::~IQueue(){};

void IQueue::load(uint opCode, uint dest, uint src1, uint src2, int immediate, short ID){
	operation.push(opCode);								//push all values given to the various queues
	destination.push((int)dest);
	sourceA.push((int)src1);							//conversions to int are safe because src1 and src2 never exceed 16 bits
	sourceB.push((int)src2);
	immediateVal.push((int)immediate);
	ROB_ID.push(ID);
};

void IQueue::unloadOldest(){
	operation.pop();						//pop oldest value from each queue
	destination.pop();
	sourceA.pop();
	sourceB.pop();
	immediateVal.pop();
	ROB_ID.pop();
};

int IQueue::size(){return ROB_ID.size();};	//since all queues are always pushed at the same time, only need to check one

uint IQueue::getOperation(){return operation.front();};
int IQueue::getDestination(){return destination.front();};
int IQueue::getSourceA(){return sourceA.front();};
int IQueue::getSourceB(){return sourceB.front();};
int IQueue::getImmediateVal(){return immediateVal.front();};
short IQueue::getROB_ID(){return ROB_ID.front();};