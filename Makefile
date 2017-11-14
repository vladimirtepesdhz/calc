DEBUG = 0

DIR_SRC = .
DIR_OBJ = ./obj
DIR_BIN = ./bin
 
SRC = $(wildcard ${DIR_SRC}/*.cpp)  
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC})) 
 
TARGET = main
 
BIN_TARGET = ${DIR_BIN}/${TARGET}

CC = g++
CFLAGS = -g -Wall 

 
${BIN_TARGET}:${OBJ}	
	$(CC) $(OBJ)  -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	@-mkdir ${DIR_OBJ} ${DIR_BIN}
	$(CC) $(CFLAGS) -c  $< -o $@

.PHONY:clean
clean:
	-del ${OBJ} ${BIN_TARGET}
	-rm -rf ${OBJ} ${BIN_TARGET}
