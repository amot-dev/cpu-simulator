#include "Pipeline.h"

Pipeline::Pipeline() : Mem(100){};
Pipeline::~Pipeline(){};

bool Pipeline::takeInput(std::string input){return Mem.loadFile(input);};

bool Pipeline::stillRunning(){return true;}

void Pipeline::fetch(){
	std::bitset<32> instructionBits(Mem.getInstruction(programCounter));	//convert the current instruction being fetched to bits
	std::bitset<4> opCode;													//create containers for each range of data
	std::bitset<5> dest, src1, src2;

	ROB.load(false, Mem.getInstruction(programCounter));

	for (int i = 26; i < 30; i++) opCode[i-26] = instructionBits[i];		//move the opCode bits into their own container
	for (int i = 21; i < 26; i++) dest[i-21] = instructionBits[i];			//move the dest bits into their own container
	for (int i = 16; i < 21; i++) src1[i-16] = instructionBits[i];			//move the src1 bits into their own container

	if (!instructionBits[31] && !instructionBits[30]){						//for the R-Type
		for (int i = 11; i < 16; i++) src2[i-11] = instructionBits[i];		//move the src2 bits into their own container
		//loads all decoded values to the instruction queue
		IQ.load(opCode.to_ulong(), dest.to_ulong(), RegFile.getRegValidity(src1.to_ulong()), src1.to_ulong(),
				RegFile.getRegValidity(src2.to_ulong()), src2.to_ulong(), ROB.getLastROB_ID());
	}
	else if (!instructionBits[31] && instructionBits[30]){					//for the I-type
		std::bitset<30> immediate;											//create an immediate container
		for (int i = 0; i < 16; i++) immediate[i] = instructionBits[i];		//move the immediate bits into their own container
		//loads all decoded values to the instruction queue (src2 is valid and set to -1 to tell execute() that an intermediate is needed)
		IQ.load(opCode.to_ulong(), dest.to_ulong(), RegFile.getRegValidity(src1.to_ulong()), src1.to_ulong(), true, -1, ROB.getLastROB_ID());
	}
	else if (instructionBits[31] && !instructionBits[30]){					//for the J-type}
		std::bitset<30> address;											//create an address container
		for (int i = 0; i < 30; i++) address[i] = instructionBits[i];		//move the address bits into their own container
		programCounter = address.to_ulong();								//set the program counter to the jump destination for the next cycle
	}
	else if (instructionBits[31] && instructionBits[30]){					//for the P-type}
		//loads all decoded values to the instruction queue (opcode 9 is for user input, dest in r2, and both srcs are NOT readable)
		IQ.load(9, 2, false, -1, false, -1, ROB.getLastROB_ID());
	}
};