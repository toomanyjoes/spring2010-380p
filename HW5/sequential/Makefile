SRCS = inout.c barneshut.c quadTree.c
OBJS = inout.o quadTree.o

CCFLAGS = -O3 -Wall -pg
LDFLAGS = -lm
MPICCFLAGS = -fast -tp barcelona-64
CC=gcc
MPICC=mpicc

all: barneshut

barneshut: barneshut.o ${OBJS}
	${CC} ${CCFLAGS} ${LDFLAGS} -o $@ barneshut.o ${OBJS}

.c.o:
	${CC} ${CCFLAGS} -c ${INCLUDES} $< -o $@

clean:
	rm -f *.o *~ barneshut

.PHONY: clean
