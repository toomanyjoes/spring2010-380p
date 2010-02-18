SRCS = inout.c mmio.c matrixmult.c
OBJS = inout.o mmio.o
INCLUDES = -I./sparse_matrix_converter/include -I./bebop_util/include
LIBDIR = -L./sparse_matrix_converter -L./bebop_util

SUBDIRS = bebop_util sparse_matrix_converter 

CCFLAGS = -Wall -O0 -g -pthread
CC=cc

all: sparse_matrix_converter matrixmult

matrixmult: matrixmult.o ${OBJS}
	@for i in $(SUBDIRS); do $(MAKE) -C $$i 2> /dev/null > /dev/null; done
	${CC} ${CCFLAGS} -o $@ matrixmult.o ${OBJS} ${LIBDIR} -lsparse_matrix_converter -lbebop_util

.c.o:
	${CC} ${CCFLAGS} -c ${INCLUDES} $< -o $@

clean:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done
	rm -f *.o *~ matrixmult

.PHONY: clean
