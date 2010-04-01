SRCS = inout.c barneshut.c quadTree.c
OBJS = inout.o quadTree.o

CCFLAGS = -O2 -Wall
LDFLAGS = -lm
MPICCFLAGS = -fast -tp barcelona-64
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
