#ifndef EXECUTE_H
#define EXECUTE_H

class Execute{
public:
	Execute();
	~Execute();

	int operationInt(unsigned opCode, int op1, int op2);
	bool operationBool(unsigned opCode, int op1, int op2);

private:
	int add(int op1, int op2);	//all self-explanatory
	int sub(int op1, int op2);
	int mult(int op1, int op2);
	int div(int op1, int op2);
	int mod(int op1, int op2);

	bool BEQ(int op1, int op2);	//returns true if equal (and branching is needed)
	bool BNE(int op1, int op2);	//returns true if not equal (and branching is needed)
};

#endif