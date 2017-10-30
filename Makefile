CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		calc.o	calculator.o

LIBS =

TARGET =	calc.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

calc.o:	calc.cpp
	$(CXX)	$(CXXFLAGS)	-c	calc.cpp

calculator.o:	calculator.cpp	calculator.h
	$(CXX)	$(CXXFLAGS)	-c	calculator.cpp

clean:
	del $(OBJS) $(TARGET)
