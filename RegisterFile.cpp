#include "RegisterFile.h"

RegisterFile::RegisterFile(){registers.reserve(16);};
RegisterFile::~RegisterFile(){};

int RegisterFile::getRegValue(int pos){return registers[pos].value;};
bool RegisterFile::getRegValidity(int pos){return registers[pos].valid;};