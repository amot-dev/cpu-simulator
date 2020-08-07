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
	std::string messages[3] = {"Command not found\n", 					//used for when the specified program was not found
							   "Invalid instruction address\n",			//used for when the program tries to access a non-existing instruction
							   "Illegal parameter; defaulting to 1\n"};	//for non-numerical parameters, as well as 0 and negative numbers
};

#endif