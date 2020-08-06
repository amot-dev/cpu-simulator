#ifndef MEMORY_H
#define MEMORY_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Memory{
public:
	Memory();
	Memory(int size);
	~Memory();

	bool loadFile(std::string filename);
	unsigned getInstruction(int pos);

private:
	std::string parseInputAsCommand(std::string);
	std::vector<unsigned> instructionMemory;
};

#endif