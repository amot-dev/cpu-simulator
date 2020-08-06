#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception{
public:
	Exception();			//initializes class and targets index = 0
	Exception(int index);	//initializes class and targets selected index
	~Exception();

	std::string what();		//returns targetted message

private:
	int index;
	std::string messages[2] = {"Command not found", 				//used for when the specified program was not found
							   "Invalid instruction address!"};		//used for when the program tries to access a non-existing instruction
};

#endif