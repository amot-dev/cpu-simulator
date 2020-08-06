#include "RegisterFile.h"

RegisterFile::RegisterFile(){
	reg temp;						//create a temp register
	temp.value = 0;					//give it value 0 and validity 1
	temp.valid = 1;
	registers.resize(16, temp);		//fill all 16 registers with that temp register
};
RegisterFile::~RegisterFile(){};

int RegisterFile::getRegValue(int pos){return registers[pos].value;};
bool RegisterFile::getRegValidity(int pos){return registers[pos].valid;};
void RegisterFile::setRegValue(int pos, int value){registers[pos].value = value;};
void RegisterFile::setRegValidity(int pos, bool validity){registers[pos].valid = validity;};