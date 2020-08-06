#include "Exception.h"

Exception::Exception() : index(0){};
Exception::Exception(int index) : index(index){};
Exception::~Exception(){};

std::string Exception::what(){
	if (index < 2) return messages[index];	//default to message 0 if index is exceeded
	else return messages[0];
};
