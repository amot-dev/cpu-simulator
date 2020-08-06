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

	int getRegValue(int pos);					//returns a register's value
	bool getRegValidity(int pos);				//returns a register's validity
	void setRegValue(int pos, int value);		//sets a register's value
	void setRegValidity(int pos, bool validity);//sets a register's validity
	void validateAll();							//validates all registers
private:
	std::vector<reg> registers;
};

#endif