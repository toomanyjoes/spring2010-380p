SRCS = inout.c mmio.c matrixmult.c
OBJS = inout.o mmio.o

CCFLAGS = -Wall -O0 -g -pthread
CC=cc

all: matrixmult

matrixmult: matrixmult.o ${OBJS}
	${CC} ${CCFLAGS} -o $@ matrixmult.o ${OBJS}

.c.o:
	${CC} ${CCFLAGS} -c $< -o $@

clean:
	rm -f *.o *~ matrixmult

.PHONY: clean
