SRCS = inout.c barneshut.c
OBJS = inout.o

CCFLAGS = -O0 -g -Wall
MPICCFLAGS = -fast -tp barcelona-64
CC=gcc
MPICC=mpicc

all: barneshut

barneshut: barneshut.o ${OBJS}
	${CC} ${CCFLAGS} -o $@ barneshut.o ${OBJS}

.c.o:
	${CC} ${CCFLAGS} -c ${INCLUDES} $< -o $@

clean:
	rm -f *.o *~ barneshut

.PHONY: clean
