GCC = g++
GCC_FLAGS = -std=c++11 -g
EXECUTABLE = exec

all: $(EXECUTABLE)

$(EXECUTABLE): genetic.o utils.o main.o
	$(GCC) $(GCC_FLAGS) -o $(PROGRAMA) genetic.o utils.o main.o

main.o: main.cpp genetic.hpp functions.hpp utils.hpp
	$(GCC) $(GCC_FLAGS) -c main.cpp

genetic.o: genetic.cpp genetic.hpp utils.hpp
	$(GCC) $(GCC_FLAGS) -c genetic.cpp

functions.o: functions.cpp functions.hpp
	$(GCC) $(GCC_FLAGS) -c utils.cpp

clean:
	rm -f *.o
	rm $(EXECUTABLE)
