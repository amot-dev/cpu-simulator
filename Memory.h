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

	void loadFile(std::string filename);
private:
	std::vector<unsigned> instructionMemory;
};

#endif