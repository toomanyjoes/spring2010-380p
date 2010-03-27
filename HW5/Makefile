SRCS = inout.c barneshut.c quadTree.c
OBJS = inout.o quadTree.o

CCFLAGS = -O0 -Wall
LDFLAGS = -lm
MPICCFLAGS = -O0 -Wall -g
CC=gcc
MPICC=mpicc

all: barneshut genBodies

barneshut: barneshut.o ${OBJS}
	${MPICC} ${MPICCFLAGS} ${LDFLAGS} -o $@ barneshut.o ${OBJS}

genBodies: genBodies.c
	${CC} ${CCFLAGS} genBodies.c -o $@

.c.o:
	${MPICC} ${MPICCFLAGS} -c ${INCLUDES} $< -o $@

clean:
	rm -f *.o *~ barneshut genBodies

.PHONY: clean
