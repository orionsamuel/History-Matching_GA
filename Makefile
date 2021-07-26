GCC = g++
GCC_FLAGS = -I include -std=c++11 -g
EXECUTABLE = exec

all: $(EXECUTABLE)

$(EXECUTABLE): functions.o genetic.o main.o
	$(GCC) $(GCC_FLAGS) -o build/$(EXECUTABLE) main.o genetic.o functions.o 

main.o: src/main.cpp genetic.o functions.o
	$(GCC) $(GCC_FLAGS) -c src/main.cpp genetic.o functions.o

genetic.o: src/genetic.cpp include/genetic.hpp functions.o
	$(GCC) $(GCC_FLAGS) -c src/genetic.cpp functions.o

functions.o: src/functions.cpp include/functions.hpp include/utils.hpp
	$(GCC) $(GCC_FLAGS) -c src/functions.cpp

clean:
	rm -f *.o
	rm $(EXECUTABLE)
