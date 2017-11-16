
DEBUG = 0
CC = g++
TARGET = main.exe
ifeq ($(DEBUG),0)
DIR_BUILD = release
CFLAGS = -O2 -Wall
else
DIR_BUILD = debug
CFLAGS = -O2 -Wall -ggdb
endif
DIR_OBJ = $(DIR_BUILD)/obj
DIR_BIN = $(DIR_BUILD)/bin
DIR_IND = $(DIR_BUILD)/ind
BIN_TARGET = ${DIR_BIN}/${TARGET}
SRC := 
OBJ := 
SRC += calc.cpp
OBJ += ${DIR_OBJ}/calc.o
SRC += Calculator.cpp
OBJ += ${DIR_OBJ}/Calculator.o
SRC += testcpp/testcpp.cpp
OBJ += ${DIR_OBJ}/testcpp.o
SRC += testc/testc.c
OBJ += ${DIR_OBJ}/testc.o
SRC += testc/testc1/testc1.c
OBJ += ${DIR_OBJ}/testc1.o

all:|createdir ${BIN_TARGET}

createdir:
	@if [ ! -d $(DIR_OBJ) ]; then\
		mkdir -p $(DIR_OBJ) ;\
	fi
	@if [ ! -d $(DIR_BIN) ]; then\
		mkdir -p $(DIR_BIN) ;\
	fi
	@if [ ! -d $(DIR_IND) ]; then\
		mkdir -p $(DIR_IND) ;\
	fi

${BIN_TARGET}:${OBJ}
	$(CC) ${OBJ} -o $@

${DIR_OBJ}/calc.o: calc.cpp config.h Calculator.h
	$(CC) $(CFLAGS) -c calc.cpp -o $@
${DIR_OBJ}/Calculator.o: Calculator.cpp Calculator.h
	$(CC) $(CFLAGS) -c Calculator.cpp -o $@
${DIR_OBJ}/testcpp.o: testcpp/testcpp.cpp testcpp/../config.h testcpp/../Calculator.h
	$(CC) $(CFLAGS) -c testcpp/testcpp.cpp -o $@
${DIR_OBJ}/testc.o: testc/testc.c
	$(CC) $(CFLAGS) -c testc/testc.c -o $@
${DIR_OBJ}/testc1.o: testc/testc1/testc1.c
	$(CC) $(CFLAGS) -c testc/testc1/testc1.c -o $@

.PHONY:clean
clean:
	-del ${OBJ} ${BIN_TARGET}
	-rm -rf ${OBJ} ${BIN_TARGET}
