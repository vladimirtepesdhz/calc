CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		calc.o	Calculator.o

LIBS =

TARGET =	calc.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

calc.o:	calc.cpp	Calculator.h
	$(CXX)	$(CXXFLAGS)	-c	calc.cpp

Calculator.o:	Calculator.cpp	Calculator.h
	$(CXX)	$(CXXFLAGS)	-c	Calculator.cpp

clean:
	del $(OBJS) $(TARGET)
