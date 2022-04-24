#!make -f
CXX=clang++-9
CXXFLAGS=-std=c++2a

OBJECTS=Stack.o allocator.o
SOURCES=Stack.cpp allocator.cpp

run:
	./$^

main: main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o main

server: server.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o server -lpthread

%.o: %.cpp
	$(CXX) $(CXXFLAGS) --compile $< -o $@

tidy:
	clang-tidy $(SOURCES) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-* --warnings-as-errors=* --

clean:
	rm -f *.o test
	rm -f StudentTest*.cpp