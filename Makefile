DEBUG = 0

ifeq ($(DEBUG),0)
CXXFLAGS =	-O2 -Wall -fmessage-length=0
OBJS =		calc.o	Calculator.o
LIBS =
TARGET =	calc.exe
else
CXXFLAGS =	-O2 -Wall -fmessage-length=0 -ggdb
OBJS =		calc.do	Calculator.do
LIBS =
TARGET =	calc.d.exe
endif


$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)


calc.o:		calc.cpp	Calculator.h
	$(CXX)	$(CXXFLAGS) -c	calc.cpp -o calc.o

Calculator.o:	Calculator.cpp	Calculator.h
	$(CXX)	$(CXXFLAGS) -c	Calculator.cpp -o Calculator.o

calc.do:	calc.cpp	Calculator.h
	$(CXX)	$(CXXFLAGS) -c	calc.cpp -o calc.do

Calculator.do:	Calculator.cpp	Calculator.h
	$(CXX)	$(CXXFLAGS) -c	Calculator.cpp -o Calculator.do

.PHONY:clean
clean:
	-del	$(OBJS)	$(TARGET) 
	-rm -rf	$(OBJS)	$(TARGET) 
