DEBUG = 0

CC = g++
DIR_SRC = .

TARGET = calc.exe

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
 
SRC = $(wildcard ${DIR_SRC}/*.cpp)  
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC})) 
IND = $(patsubst %.cpp,${DIR_IND}/%.d,$(notdir ${SRC})) 
 
 
BIN_TARGET = ${DIR_BIN}/${TARGET}


all:|createdir IND_TARGET ${BIN_TARGET}
	
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

IND_TARGET:${IND}
	
${DIR_IND}/%.d:${DIR_SRC}/%.cpp
	$(CC) -MM $< | sed 's,\($*\)\.o[ :]\?,${DIR_OBJ}/\1\.o $@ :,g' >$@

 
${BIN_TARGET}:${OBJ}	
	$(CC) $(OBJ)  -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	$(CC) $(CFLAGS) -c  $< -o $@
-include $(IND)


.PHONY:clean
clean:
	-del ${IND} ${OBJ} ${BIN_TARGET}
	-rm -rf ${IND} ${OBJ} ${BIN_TARGET}
