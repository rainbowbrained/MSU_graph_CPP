COMPILER = g++
COMPILER_FLAGS = -std=c++17 -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -Wuninitialized
LINK_COMPILER_FLAGS = -fsanitize=undefined -g -o

spm: main.o densematrix.o sparsematrix.o iterator.o graph.o
	$(COMPILER) $(LINK_COMPILER_FLAGS) $@ $^ 
main.o: main.cpp matrix.hpp densematrix.hpp sparsematrix.hpp
	$(COMPILER) $(COMPILER_FLAGS) -g -c main.cpp matrix.hpp densematrix.hpp sparsematrix.hpp graph.cpp
	
densematrix.o: densematrix.cpp densematrix.hpp
	$(COMPILER) $(COMPILER_FLAGS) -g -c densematrix.cpp -o $@
	
sparsematrix.o: sparsematrix.cpp sparsematrix.hpp
	$(COMPILER) $(COMPILER_FLAGS) -g -c sparsematrix.cpp -o $@

iterator.o: matrix.hpp
	$(COMPILER) $(COMPILER_FLAGS) -g -c iterator.cpp -o $@
	
graph.o: graph.hpp matrix.hpp
	$(COMPILER) $(COMPILER_FLAGS) -g -c graph.cpp -o $@

clear:
	rm *.o *.gch spm
