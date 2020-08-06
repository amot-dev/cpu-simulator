#define DEBUG 1	//enables or disables debug mode

#include <iostream>

#include "Execute.h"
#include "FetchUnit.h"
#include "Instruction.h"
#include "IQueue.h"
#include "Memory.h"
#include "Pipeline.h"
#include "RegisterFile.h"
#include "Statistics.h"

int main(){
	std::cout << "Type 'exit' to terminate this application\n";

	Pipeline Pipe;
	Memory Mem;

	std::string dir = "~/samples";
	std::string input;
	while(true){
		std::cout << "user@cpusim:" + dir + "$ ";
		getline(std::cin, input);

		if (input == "exit") break;

		//for immersion (you can ignore most of this)
		if (dir == "/"){
			if (input == "ls") std::cout << "home\n";
			else if (input[0] == 'c' && input[1] == 'd'){
				if (input == "cd") dir = "~";
				else if (input == "cd ..");
				else if (input == "cd home") dir = "/home";
				else{
					input.erase(0,2);
					std::cout << "-bash: cd: " + input + ": No such file or directory";
				};
			};
		}
		else if (dir == "/home"){
			if (input == "ls") std::cout << "user\n";
			else if (input[0] == 'c' && input[1] == 'd'){
				if (input == "cd") dir = "~";
				else if (input == "cd ..") dir = "/";
				else if (input == "cd user") dir = "~";
				else if (input == "cd user/samples") dir = "~/samples";
				else{
					input.erase(0,2);
					std::cout << "-bash: cd: " + input + ": No such file or directory";
				};
			};
		}
		else if (dir == "~"){
			if (input == "ls") std::cout << "samples\n";
			else if (input[0] == 'c' && input[1] == 'd'){
				if (input == "cd") dir = "~";
				else if (input == "cd ..") dir = "/home";
				else if (input == "cd ../..") dir = "/";
				else if (input == "cd samples") dir = "~/samples";
				else{
					input.erase(0,2);
					std::cout << "-bash: cd: " + input + ": No such file or directory";
				};
			};
		}
		else if (dir == "~/samples"){
			if (input == "ls") std::cout << "factorial\nfibonacci\ninst_mem\n";
			else if (input[0] == 'c' && input[1] == 'd'){
				if (input == "cd") dir = "~";
				else if (input == "cd ..") dir = "~";
				else if (input == "cd ../..") dir = "/home";
				else if (input == "cd ../../..") dir = "/";
				else{
					input.erase(0,2);
					std::cout << "-bash: cd: " + input + ": No such file or directory";
				};
			}
			else {
				//here is the actual program now
			};
		};

	};
	return 0;
};