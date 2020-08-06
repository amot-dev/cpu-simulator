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

	std::string parseInputAsCommand(std::string);

private:
	void loadFile(std::string filename);

	std::vector<unsigned> instructionMemory;
};

#endif