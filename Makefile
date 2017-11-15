DEBUG = 0

CC = g++
DIR_SRC = .
TARGET = calc.exe

ifeq ($(DEBUG),0)
DIR_OBJ = release
DIR_BIN = release
CFLAGS = -O2 -Wall 
else
DIR_OBJ = debug
DIR_BIN = debug
CFLAGS = -O2 -Wall -ggdb
endif
 
SRC = $(wildcard ${DIR_SRC}/*.cpp)  
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC})) 
 
 
BIN_TARGET = ${DIR_BIN}/${TARGET}


all:|createdir ${BIN_TARGET}

createdir:
	@if [ ! -d $(DIR_BIN) ] ; then\
		mkdir $(DIR_BIN) ;\
	fi
	@if [ ! -d $(DIR_OBJ) ] ; then\
		mkdir $(DIR_OBJ) ;\
	fi
 
${BIN_TARGET}:${OBJ}	
	$(CC) $(OBJ)  -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

.PHONY:clean
clean:
	-del ${OBJ} ${BIN_TARGET}
	-rm -rf ${OBJ} ${BIN_TARGET}
