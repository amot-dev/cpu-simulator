#include "Memory.h"

Memory::Memory(int size){instructionMemory.reserve(size);}; //reserve needed amount of memory

void Memory::loadFile(std::string filename){
	std::ifstream File;
	File.open(filename);			//attempt to open file

	if (!File){						//if the file was not correctly opened
		std::cout << "Error in opening file...\n";
		exit(1);
	};

	std::string data;								//temp storage for data read
	while (getline(File, data, ',')){				//while there are values left to grab, grab them
		std::string temp = data;data = "";			//move data to a temp string, then empty data
		for (int i=0; i < temp.size();i++) if (isdigit(temp[i])) data += temp[i];	//go through data and keep only digits
		instructionMemory.push_back(stoul(data));	//add the unsigned int to instruction memory
	};
};