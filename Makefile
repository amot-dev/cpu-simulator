OBJS	= main.o Exception.o Execute.o IQueue.o Memory.o Pipeline.o RegisterFile.o ReorderBuffer.o Statistics.o
SOURCE	= main.cpp Exception.cpp Execute.cpp IQueue.cpp Memory.cpp Pipeline.cpp RegisterFile.cpp ReorderBuffer.cpp Statistics.cpp
OUT	    = cpu
CC	    = g++ -std=c++11
FLAGS	= -g -c 

all: $(OBJS)
		$(CC) -o $(OUT) $(OBJS)

main.o: main.cpp
		$(CC) $(FLAGS) main.cpp
Exception.o: Exception.cpp
		$(CC) $(FLAGS) Exception.cpp
Execute.o: Execute.cpp
		$(CC) $(FLAGS) Execute.cpp
IQueue.o: IQueue.cpp
		$(CC) $(FLAGS) IQueue.cpp
Memory.o: Memory.cpp
		$(CC) $(FLAGS) Memory.cpp
Pipeline.o: Pipeline.cpp
		$(CC) $(FLAGS) Pipeline.cpp
RegisterFile.o: RegisterFile.cpp
		$(CC) $(FLAGS) RegisterFile.cpp
ReorderBuffer.o: ReorderBuffer.cpp
		$(CC) $(FLAGS) ReorderBuffer.cpp
Statistics.o: Statistics.cpp
		$(CC) $(FLAGS) Statistics.cpp

clean:
		rm -f $(OBJS) $(OUT)