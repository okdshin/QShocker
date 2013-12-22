CXX = clang++ -std=c++11 -D__STRICT_ANSI__
CXXFLAGS = -Wall -g -D SHOCKER_UNIT_TEST
INCLUDES = 
LIBS = -lboost_system -lopencv_highgui -lopencv_core
OBJS = shocker.o
PROGRAM = shocker.out

all:$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(INCLUDES) $(LIBS) -o $(PROGRAM)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -c $<

.PHONY: clean
clean:
	rm -f *o $(PROGRAM)
