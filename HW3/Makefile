SRCS = inout.c matrixmult.c verify.c
OBJS = inout.o verify.o
INCLUDES = -I./sparse_matrix_converter/include -I./bebop_util/include
LIBDIR = -L./sparse_matrix_converter -L./bebop_util

SUBDIRS = bebop_util sparse_matrix_converter 

CCFLAGS = -Wall -O0 -g -pthread -static
CC=gcc

all: sparse_matrix_converter matrixmult

matrixmult: sparse_matrix_converter matrixmult.o ${OBJS}
	${CC} ${CCFLAGS} -o $@ matrixmult.o ${OBJS} ${LIBDIR} -lsparse_matrix_converter -lbebop_util

sparse_matrix_converter:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i; done

genVector: genVector.c
	${CC} ${CCFLAGS} genVector.c -o $@

.c.o:
	${CC} ${CCFLAGS} -c ${INCLUDES} $< -o $@

clean:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm -f *.o *~ matrixmult genVector

.PHONY: clean sparse_matrix_converter
