#define DEBUG //comment/uncomment to disable/enable debug mode
#define FETCH_WIDTH 2
#define EXECUTE_WIDTH 2
#define COMMIT_WIDTH 2

#include "Pipeline.h"

Pipeline::Pipeline() : Mem(estimatedInstructionCount){};
Pipeline::~Pipeline(){};

bool Pipeline::takeInput(std::string input){return Mem.loadFile(input);};
bool Pipeline::stillRunning(){return !ROB.empty();};

void Pipeline::doClockCycle(){
	std::list<short> temp = ROB.getROB_IDs();	//get all ROB_IDs in the buffer
	for (auto it = temp.begin(); it != temp.end(); it++) Stats.incrementLatency(*it);	//for each ID present, increment its latency
	Stats.pushLatestThroughput();				//store amount of instructions executed during this cycle
	Stats.incrementCycles();					//go to the next cycle
};

void Pipeline::printCycleDebug(){
	#ifdef DEBUG
		std::cout << "\n\033[32m==== Clock Cycle " << Stats.getCycle() << " ====\033[0m\n";
	#endif
};

void Pipeline::fetch(){for(int w = 0; w < FETCH_WIDTH; w++) if (IQ.size() < FETCH_WIDTH) {	//fetch as many times as min(FETCH_WIDTH, IQ.size())
	if (noFetch) return;													//stops fetching if noFetch is true
	if (!Mem.instructionExists(programCounter)) return;						//stops fetching if the specified instruction does not exist
	std::bitset<32> instructionBits(Mem.getInstruction(programCounter));	//convert the current instruction being fetched to bits
	std::bitset<4> opCode;													//create containers for each range of data
	std::bitset<5> dest, src1, src2;

	ROB.load(Mem.getInstruction(programCounter));							//load instructions to ROB

	#ifdef DEBUG
		std::cout << "\033[34mFetching\033[0m PC[" << programCounter << "] (" << instructionBits.to_string() << ")\n";
		std::cout << "\033[34mAssigning\033[0m to ROB_ID[" << ROB.getLastROB_ID() << "]\n";
	#endif

	for (int i = 26; i < 30; i++) opCode[i-26] = instructionBits[i];		//move the opCode bits into their own container
	for (int i = 21; i < 26; i++) dest[i-21] = instructionBits[i];			//move the dest bits into their own container
	for (int i = 16; i < 21; i++) src1[i-16] = instructionBits[i];			//move the src1 bits into their own container

	if (!instructionBits[31] && !instructionBits[30]){						//for the R-Type
		for (int i = 11; i < 16; i++) src2[i-11] = instructionBits[i];		//move the src2 bits into their own container
		//loads all decoded values to the instruction queue (no immediate value)
		IQ.load(opCode.to_ulong(), dest.to_ulong(), src1.to_ulong(), src2.to_ulong(), 0, ROB.getLastROB_ID());
		programCounter++;													//increment program counter (only for R-Type and P-type)
	}
	else if (!instructionBits[31] && instructionBits[30]){					//for the I-type
		std::bitset<30> immediate;											//create an immediate container
		for (int i = 0; i < 16; i++) immediate[i] = instructionBits[i];		//move the immediate bits into their own container
		//loads all decoded values to the instruction queue (src2 is set to -1 to tell execute() that an intermediate is needed)
		IQ.load(opCode.to_ulong(), dest.to_ulong(), src1.to_ulong(), -1, immediate.to_ulong(), ROB.getLastROB_ID());
		if (opCode.to_ulong() == 5 || opCode.to_ulong() == 6) noFetch = true;	//set noFetch to true if a possible branch is detected
		programCounter++;													//if no branch, then increment is necessary; if branch, increment doesn't hurt
	}
	else if (instructionBits[31] && !instructionBits[30]){					//for the J-type}
		std::bitset<30> address;											//create an address container
		for (int i = 0; i < 30; i++) address[i] = instructionBits[i];		//move the address bits into their own container
		//loads all decoded values to the instruction queue (opcode 10 is for jump, dest is r15 (should not be used), srcs are -1)
		IQ.load(10, 15, -1, -1, 0, ROB.getLastROB_ID());
		programCounter = address.to_ulong();								//set the program counter to the jump destination for the next cycle
	}
	else if (instructionBits[31] && instructionBits[30]){					//for the P-type}
		//loads all decoded values to the instruction queue (opcode 9 is for user input, dest is r2, srcs are -1)
		IQ.load(9, 2, -1, -1, 0, ROB.getLastROB_ID());
		programCounter++;
	};
};};

void Pipeline::execute(){for(int w = 0; w < EXECUTE_WIDTH; w++){//execute as many times as the EXECUTE_WIDTH
	if (!IQ.size()) return;		//stops executing if there are no instructions to execute

	Execute ALU;				//create an ALU
	int op1;					//create the operators
	int op2;

	//load op1 and op2
	if (IQ.getSourceA() == -1 && IQ.getOperation() != 10){		//if source A is -1, (and opcode is not jump) then user input is required (source B does not matter)
		#ifdef DEBUG
			std::cout << "\033[31mExecuting\033[0m ROB_ID[" << IQ.getROB_ID() << "] (r2 : param)\n";
		#endif
		std::cout << "\033[1m\033[37mPlease enter a parameter: \033[0m";
		std::cin >> op1;
		try {if (std::cin.fail() || op1 <= 0) throw Exception(2);}	//throw exception if input is not an integer or is <= 0
		catch(Exception error){std::cout << error.what();op1 = 1;}	//catch exception and set op1 to default of 1
		std::cin.clear();					//clear cin buffer (for getline in main.cpp)
      	std::cin.ignore(10000,'\n');
		RegFile.setRegValue(2, op1);		//set r2 to op1
		RegFile.setRegValidity(2, false);	//prevent further reads from r2 this cycle
	}
	else {				
		if (RegFile.getRegValidity(IQ.getSourceA())){
			#ifdef DEBUG
				std::cout << "\033[31mExecuting\033[0m ROB_ID[" << IQ.getROB_ID() << "] (r" << IQ.getDestination();
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
		RegFile.setRegValidity(IQ.getDestination(), false);											//prevent further reads from destination register this cycle
	}
	else if (IQ.getOperation() == 5 || IQ.getOperation() == 6){
		if (ALU.operationBool(IQ.getOperation(), RegFile.getRegValue(IQ.getDestination()), op1)) programCounter = IQ.getImmediateVal();	//for BEQ and BNE, if true, jump to address in immediate
	}
	else if (IQ.getOperation() == 7){
		#ifdef DEBUG
			std::cout << ": mov)\n";
		#endif
		RegFile.setRegValue(IQ.getDestination(), op1);			//move op1 directly to the destination register
		RegFile.setRegValidity(IQ.getDestination(), false);		//prevent further reads from destination register this cycle
	}
	else if (IQ.getOperation() == 8){
		#ifdef DEBUG
			std::cout << ": print)\n";
		#endif
		std::cout << "\033[1m\033[37mFinal result is: \033[0m" << op1 << "\n";	//print operation prints op1 directly to user
	};

	//post operation, it is time to remove the instruction from the IQ and set it as valid in the ROB
	ROB.setValidity(IQ.getROB_ID());	//set the instruction with the current ROB_ID to valid in the ROB
	IQ.unloadOldest();					//unload the instructions from the IQ
	#ifdef DEBUG
		std::cout << "\033[36mRegisters:\033[0m\n";
		for (int i = 0; i < 16; i++){
			std::cout << "\t" << std::setw(4) << "\033[33mr" << std::setw(2) << std::setfill('0') << i << std::setfill(' ')
					  << "\033[0m: [" << std::setw(2) << std::setfill('0') << RegFile.getRegValue(i) << std::setfill(' ') << "] ";
			if ((i+1) % 4 == 0) std::cout << "\n";
		};
	#endif
	
	Stats.incrementLatestThroughput();	//increment throughput for each consecutive execution
}};

void Pipeline::commit(){for(int w = 0; w < COMMIT_WIDTH; w++){	//commit as many times as the COMMIT_WIDTH
	if (ROB.empty()) return;		//if ROB is empty, stop execution
	ROB.unloadOldestIfValid();		//unload the oldest entry in the ROB if it is valid
	RegFile.validateAll();			//writing is done, so validate all registers for reading
	noFetch = false;				//execution is complete, so the next set of instructions may be fetched
}};