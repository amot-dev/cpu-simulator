#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Exception.h"

class Memory{
public:
	Memory();
	Memory(int size);
	~Memory();

	bool loadFile(std::string filename);	//loads the file given
	unsigned getInstruction(int pos);		//returns the instructions at a pos
	bool instructionExists(int pos);		//returns whether or not the specified instruction exists

private:
	std::string parseInputAsCommand(std::string);
	std::vector<unsigned> instructionMemory;
};

#endif