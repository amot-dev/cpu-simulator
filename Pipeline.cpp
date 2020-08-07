#define DEBUG //comment/uncomment to disable/enable debug mode

#include "Pipeline.h"

Pipeline::Pipeline() : Mem(25){};
Pipeline::~Pipeline(){};

bool Pipeline::takeInput(std::string input){return Mem.loadFile(input);};
bool Pipeline::stillRunning(){return !ROB.empty();};

void Pipeline::fetch(){
	if (noFetch) return;													//stops fetching if noFetch is true
	if (!Mem.instructionExists(programCounter)) return;						//stops fetching if the specified instruction does not exist
	std::bitset<32> instructionBits(Mem.getInstruction(programCounter));	//convert the current instruction being fetched to bits
	std::bitset<4> opCode;													//create containers for each range of data
	std::bitset<5> dest, src1, src2;

	#ifdef DEBUG
		std::cout << "[" << programCounter << "] " << instructionBits.to_string();
	#endif

	ROB.load(Mem.getInstruction(programCounter));							//load instructions to ROB

	for (int i = 26; i < 30; i++) opCode[i-26] = instructionBits[i];		//move the opCode bits into their own container
	for (int i = 21; i < 26; i++) dest[i-21] = instructionBits[i];			//move the dest bits into their own container
	for (int i = 16; i < 21; i++) src1[i-16] = instructionBits[i];			//move the src1 bits into their own container

	if (!instructionBits[31] && !instructionBits[30]){						//for the R-Type
		#ifdef DEBUG
			std::cout << " (R-Type)\n";
		#endif
		for (int i = 11; i < 16; i++) src2[i-11] = instructionBits[i];		//move the src2 bits into their own container
		//loads all decoded values to the instruction queue (no immediate value)
		IQ.load(opCode.to_ulong(), dest.to_ulong(), src1.to_ulong(), src2.to_ulong(), 0, ROB.getLastROB_ID());
		programCounter++;													//increment program counter (only for R-Type and P-type)
	}
	else if (!instructionBits[31] && instructionBits[30]){					//for the I-type
		#ifdef DEBUG
			std::cout << " (I-Type)\n";
		#endif
		std::bitset<30> immediate;											//create an immediate container
		for (int i = 0; i < 16; i++) immediate[i] = instructionBits[i];		//move the immediate bits into their own container
		//loads all decoded values to the instruction queue (src2 is set to -1 to tell execute() that an intermediate is needed)
		IQ.load(opCode.to_ulong(), dest.to_ulong(), src1.to_ulong(), -1, immediate.to_ulong(), ROB.getLastROB_ID());
		if (opCode.to_ulong() == 5 || opCode.to_ulong() == 6) noFetch = true;	//set noFetch to true if a possible branch is detected
		programCounter++;													//if no branch, then increment is necessary; if branch, increment doesn't hurt
	}
	else if (instructionBits[31] && !instructionBits[30]){					//for the J-type}
		#ifdef DEBUG
			std::cout << " (J-Type)\n";
		#endif
		std::bitset<30> address;											//create an address container
		for (int i = 0; i < 30; i++) address[i] = instructionBits[i];		//move the address bits into their own container
		//loads all decoded values to the instruction queue (opcode 10 is for jump, dest is r15 (should not be used), srcs are -1)
		IQ.load(10, 15, -1, -1, 0, ROB.getLastROB_ID());
		programCounter = address.to_ulong();								//set the program counter to the jump destination for the next cycle
	}
	else if (instructionBits[31] && instructionBits[30]){					//for the P-type}
		#ifdef DEBUG
			std::cout << " (P-Type)\n";
		#endif
		//loads all decoded values to the instruction queue (opcode 9 is for user input, dest is r2, srcs are -1)
		IQ.load(9, 2, -1, -1, 0, ROB.getLastROB_ID());
		programCounter++;
	};
};

void Pipeline::execute(){
	if (IQ.empty()) return;		//stops executing if there are no instructions to execute

	Execute ALU;				//create an ALU
	int op1;					//create the operators
	int op2;

	//load op1 and op2
	if (IQ.getSourceA() == -1 && IQ.getOperation() != 10){		//if source A is -1, (and opcode is not jump) then user input is required (source B does not matter)
		#ifdef DEBUG
			std::cout << "r2 : param\n";
		#endif
		std::cout << "Please enter a parameter: ";
		std::cin >> op1;
		try {if (std::cin.fail() || op1 <= 0) throw Exception(2);}	//throw exception if input is not an integer or is <= 0
		catch(Exception error){std::cout << error.what();op1 = 1;}	//catch exception and set op1 to default of 1
		std::cin.clear();					//clear cin buffer (for getline in main.cpp)
      	std::cin.ignore(10000,'\n');
		RegFile.setRegValue(2, op1);		//set r2 to op1
		RegFile.setRegValidity(2, false);	//prevent further writes to r2 this cycle
	}
	else {				
		if (RegFile.getRegValidity(IQ.getSourceA())){
			#ifdef DEBUG
				std::cout << "r" << IQ.getDestination();
				if (IQ.getOperation() != 8) std::cout << ", r" << IQ.getSourceA();
			#endif
			op1 = RegFile.getRegValue(IQ.getSourceA());		//if source A is not -1, it is safe to its register into op1 if valid	
		}
		else {noFetch = true;return;}	//if not valid, stop fetching new instructions and stop executing

		if (IQ.getSourceB() == -1){
			if (IQ.getOperation() != 5 && IQ.getOperation() != 6){
				#ifdef DEBUG
					std::cout << ", i";
				#endif
			};
			op2 = IQ.getImmediateVal();	//if source B is -1, op2 is an immediate
		}
		else {					
			if (RegFile.getRegValidity(IQ.getSourceB())){
				#ifdef DEBUG
					if (IQ.getOperation() != 7 && IQ.getOperation() != 8) std::cout << ", r" << IQ.getSourceB();
				#endif	
				op2 = RegFile.getRegValue(IQ.getSourceB());	//if source B is not -1, it is safe to put its register into op2 if valid	
			}
			else {noFetch = true;return;}	//if not valid, stop fetching new instructions and stop executing
		};
	};
	//now that op1 and op2 are set, it's time to figure out the operation
	if (IQ.getOperation() >= 0 && IQ.getOperation() < 5){
		RegFile.setRegValue(IQ.getDestination(),ALU.operationInt(IQ.getOperation(), op1, op2));		//complete operation and store in destination register
		RegFile.setRegValidity(IQ.getDestination(), false);											//prevent further writes to destination register this cycle
	}
	else if (IQ.getOperation() == 5 || IQ.getOperation() == 6){
		if (ALU.operationBool(IQ.getOperation(), RegFile.getRegValue(IQ.getDestination()), op1)) programCounter = IQ.getImmediateVal();	//for BEQ and BNE, if true, jump to address in immediate
	}
	else if (IQ.getOperation() == 7){
		#ifdef DEBUG
			std::cout << ": mov\n";
		#endif
		RegFile.setRegValue(IQ.getDestination(), op1);			//move op1 directly to the destination register
		RegFile.setRegValidity(IQ.getDestination(), false);		//prevent further writes to destination register this cycle
	}
	else if (IQ.getOperation() == 8){
		#ifdef DEBUG
			std::cout << ": print\n";
		#endif
		std::cout << "Final result is: " << op1 << "\n";				//print operation prints op1 directly to user
	};

	//post operation, it is time to remove the instruction from the IQ and set it as valid in the ROB
	ROB.setValidity(IQ.getROB_ID());	//set the instruction with the current ROB_ID to valid in the ROB
	IQ.unloadOldest();					//unload the instructions from the IQ
	#ifdef DEBUG
		for (int i = 0; i < 16; i++){
			std::cout << std::setw(4) << "r" << std::setw(2) << std::setfill('0') << i << std::setfill(' ')
					  << ": [" << std::setw(2) << std::setfill('0') << RegFile.getRegValue(i) << std::setfill(' ') << "] ";
			if ((i+1) % 4 == 0) std::cout << "\n";
		};
	#endif
};

void Pipeline::commit(){
	if (ROB.empty()) return;		//if ROB is empty, stop execution
	ROB.unloadOldestIfValid();		//unload the oldest entry in the ROB if it is valid
	RegFile.validateAll();			//writing is done, so validate all registers for reading
	noFetch = false;				//execution is complete, so the next set of instructions may be fetched
};