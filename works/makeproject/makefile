DIR_LIB = ../libs
DIR_SRC = ../source
DIR_OBJ = ../makeproject
DIR_BIN = ../target

SRC = $(wildcard ${DIR_SRC}/*.cpp)
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC}))
TARGET = game
BIN_TARGET = ${DIR_BIN}/${TARGET}

CC = g++
CFLAGS= -lpthread  -Wall -g -I${DIR_LIB}

${BIN_TARGET}:${OBJ}
	$(CC) $(OBJ) -o $@ 

${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY:clean
clean:
	rm  ${DIR_OBJ}/*.o
