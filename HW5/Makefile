SRCS = inout.c barneshut.c
OBJS = inout.o

CCFLAGS = -O2 -Wall
MPICCFLAGS = -fast -tp barcelona-64
CC=gcc
MPICC=mpicc

all: barneshut

barneshut: barneshut.o ${OBJS}
	${MPICC} ${MPICCFLAGS} -o $@ barneshut.o ${OBJS}

.c.o:
	${MPICC} ${MPICCFLAGS} -c ${INCLUDES} $< -o $@

clean:
	rm -f *.o *~ barneshut

.PHONY: clean
