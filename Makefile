GCC = g++
GCC_FLAGS = -std=c++11 -g
EXECUTABLE = exec

all: $(EXECUTABLE)

$(EXECUTABLE): functions.o genetic.o main.o
	$(GCC) $(GCC_FLAGS) -o $(EXECUTABLE) main.o genetic.o functions.o 

main.o: main.cpp genetic.o functions.o
	$(GCC) $(GCC_FLAGS) -c main.cpp genetic.o functions.o

genetic.o: genetic.cpp genetic.hpp functions.o
	$(GCC) $(GCC_FLAGS) -c genetic.cpp functions.o

functions.o: functions.cpp functions.hpp utils.hpp
	$(GCC) $(GCC_FLAGS) -c functions.cpp

clean:
	rm -f *.o
	rm $(EXECUTABLE)
