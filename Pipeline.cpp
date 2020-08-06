#include "Pipeline.h"

Pipeline::Pipeline() : Mem(100){};
Pipeline::~Pipeline(){};

bool Pipeline::takeInput(std::string input){return Mem.loadFile(input);};
bool Pipeline::stillRunning(){return !ROB.empty();};

void Pipeline::fetch(){
	if (noFetch) return;													//stops fetching if noFetch is true

	std::bitset<32> instructionBits(Mem.getInstruction(programCounter));	//convert the current instruction being fetched to bits
	std::bitset<4> opCode;													//create containers for each range of data
	std::bitset<5> dest, src1, src2;

	ROB.load(Mem.getInstruction(programCounter));							//load instructions to ROB

	for (int i = 26; i < 30; i++) opCode[i-26] = instructionBits[i];		//move the opCode bits into their own container
	for (int i = 21; i < 26; i++) dest[i-21] = instructionBits[i];			//move the dest bits into their own container
	for (int i = 16; i < 21; i++) src1[i-16] = instructionBits[i];			//move the src1 bits into their own container

	if (!instructionBits[31] && !instructionBits[30]){						//for the R-Type
		for (int i = 11; i < 16; i++) src2[i-11] = instructionBits[i];		//move the src2 bits into their own container
		//loads all decoded values to the instruction queue (no immediate value)
		IQ.load(opCode.to_ulong(), dest.to_ulong(), src1.to_ulong(), src2.to_ulong(), 0, ROB.getLastROB_ID());
	}
	else if (!instructionBits[31] && instructionBits[30]){					//for the I-type
		std::bitset<30> immediate;											//create an immediate container
		for (int i = 0; i < 16; i++) immediate[i] = instructionBits[i];		//move the immediate bits into their own container
		//loads all decoded values to the instruction queue (src2 is set to -1 to tell execute() that an intermediate is needed)
		IQ.load(opCode.to_ulong(), dest.to_ulong(), src1.to_ulong(), -1, immediate.to_ulong(), ROB.getLastROB_ID());
		if (opCode.to_ulong() == 5 || opCode.to_ulong() == 6) noFetch = true;	//set noFetch to true if a possible branch is detected
	}
	else if (instructionBits[31] && !instructionBits[30]){					//for the J-type}
		std::bitset<30> address;											//create an address container
		for (int i = 0; i < 30; i++) address[i] = instructionBits[i];		//move the address bits into their own container
		programCounter = address.to_ulong();								//set the program counter to the jump destination for the next cycle
	}
	else if (instructionBits[31] && instructionBits[30]){					//for the P-type}
		//loads all decoded values to the instruction queue (opcode 9 is for user input, dest is r2, srcs are -1)
		IQ.load(9, 2, -1, -1, 0, ROB.getLastROB_ID());
	}
};

void Pipeline::execute(){
	if (IQ.empty()) return;		//stops executing if there are no instructions to execute

	Execute ALU;				//create an ALU
	int op1;					//create the operators
	int op2;

	//load op1 and op2
	if (IQ.getSourceA() == -1){				//if source A is -1, then user input is required (source B does not matter)
		std::cout << "Please enter a parameter: ";
		std::cin >> op1;
		std::cin.clear();					//clear cin buffer (for getline in main.cpp)
      	std::cin.ignore(10000,'\n');
		RegFile.setRegValue(2, op1);		//set r2 to op1
		RegFile.setRegValidity(2, false);	//prevent further writes to r2 this cycle
	}
	else {									//if source A is not -1, it is safe to put it into op1 if valid				
		if (RegFile.getRegValidity(IQ.getSourceA())) op1 = IQ.getSourceA();
		else noFetch = true;return;			//if not valid, stop fetching new instructions and stop executing

		if (IQ.getSourceB() == -1) op2 = IQ.getImmediateVal();	//if source B is -1, op2 is an immediate
		else {								//if source B is not -1, it is safe to put it into op2 if valid	
			if (RegFile.getRegValidity(IQ.getSourceB())) op2 = IQ.getSourceB();
			else noFetch = true;return;		//if not valid, stop fetching new instructions and stop executing
		};
	};
	
	//now that op1 and op2 are set, it's time to figure out the operation
	if (IQ.getOperation() >= 0 && IQ.getOperation() < 5){
		RegFile.setRegValue(IQ.getDestination(),ALU.operationInt(IQ.getOperation(), op1, op2));		//complete operation and store in destination register
		RegFile.setRegValidity(IQ.getDestination(), false);											//prevent further writes to destination register this cycle
	}
	else if (IQ.getOperation() == 5 || IQ.getOperation() == 6){
		if (ALU.operationBool(IQ.getOperation(), op1, op2)) programCounter = IQ.getImmediateVal();	//for BEQ and BNE, if true, jump to address in immediate
	}
	else if (IQ.getOperation() == 7){
		RegFile.setRegValue(IQ.getDestination(), op1);			//move op1 directly to the destination register
		RegFile.setRegValidity(IQ.getDestination(), false);		//prevent further writes to destination register this cycle
	}
	else if (IQ.getOperation() == 8) std::cout << "Final result is: " << op1 << "\n";				//print operation prints op1 directly to user

	//post operation, it is time to remove the instruction from the IQ and set it as valid in the ROB
	ROB.setValidity(IQ.getROB_ID());	//set the instruction with the current ROB_ID to valid in the ROB
	IQ.unloadOldest();					//unload the instructions from the IQ
};

void Pipeline::commit(){
	if (ROB.empty()) return;		//if ROB is empty, stop execution
	ROB.unloadOldestIfValid();		//unload the oldest entry in the ROB if it is valid
};