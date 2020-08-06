#include "Execute.h"

Execute::Execute(){};
Execute::~Execute(){};

int Execute::operation(unsigned opCode, int op1, int op2){
	switch (opCode){	//depending on opCode, perform a certain operation (break not required since the function returns on each operation)
		case 0: return add(op1, op2);
		case 1: return sub(op1, op2);
		case 2: return mult(op1, op2);
		case 3: return div(op1, op2);
		case 4: return mod(op1, op2);
		case 5: return BEQ(op1, op2);
		case 6: return BNE(op1, op2);	
		default: return 0;
	};
};

int Execute::add(int op1, int op2){return op1 + op2;};
int Execute::sub(int op1, int op2){return op1 - op2;};
int Execute::mult(int op1, int op2){return op1 * op2;};
int Execute::div(int op1, int op2){return op1 / op2;};
int Execute::mod(int op1, int op2){return op1 % op2;};

bool Execute::BEQ(int op1, int op2){
	if (sub(op1, op2) == 0) return true;	//if values are equal, return true
	else return false;						//else, return false
};
bool Execute::BNE(int op1, int op2){return !BEQ(op1, op2);};	//return the opposite of BEQ