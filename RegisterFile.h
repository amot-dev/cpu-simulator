#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <vector>

struct reg{
	int value = 0;
	bool valid = 1;
};

class RegisterFile{
public:
	RegisterFile();
	~RegisterFile();

	int getRegValue(int pos);
	bool getRegValidity(int pos);
private:
	std::vector<reg> registers;
};

#endif