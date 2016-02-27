# the compiler to use.
CC=g++
src =$(wildcard *.cpp)
OBJS =$(src:.cpp=.o)

LIBB =/usr/lib/ /usr/lib/x86_64-linux-gnu
LDFLAGS = $(addprefix -L,$(LIBB))
LDFLAGS += -lboost_thread -lpthread -lboost_system

LIBS =

INCLUDES  =
HEADERS   = GenericDelegate.h
EXECUTABLE=GenericDelegateTest

# options I'll pass to the compiler.
CXXFLAGS =-std=c++11 -m64 -static -Wall -g -c ${INCLUDES}
DEBUG    =-std=c++11 -m64 -static -Wall -g -c ${INCLUDES}

all: $(EXECUTABLE)

$(EXECUTABLE): $(EXECUTABLE).o
	$(CC) $^ $(LDFLAGS) -o $@ $(LIBS)

$(EXECUTABLE).o: $(EXECUTABLE).cpp $(HEADERS)
	$(CC) $(CXXFLAGS) $(EXECUTABLE).cpp
	
debug: CXXFLAGS=$(DEBUG)
debug: all

.PHONY: clean all debug lib
clean:
	rm -f *.o *~ xmltest ./lib/*.a $(EXECUTABLE)

