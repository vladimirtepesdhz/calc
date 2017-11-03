CXXFLAGS =	-O2 -Wall -fmessage-length=0

OBJS =		calc.o	Calculator.o

LIBS =

TARGET =	calc.exe


$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)


calc.o:	calc.cpp	Calculator.h
	$(CXX)	$(CXXFLAGS) -c	calc.cpp

Calculator.o:	Calculator.cpp	Calculator.h
	$(CXX)	$(CXXFLAGS) -c	Calculator.cpp


.PHONY:clean
clean:
ifeq ($(shell uname),Linux)
	-rm -rf	*.o	$(TARGET) 
else
	-del	*.o	$(TARGET) 
endif
