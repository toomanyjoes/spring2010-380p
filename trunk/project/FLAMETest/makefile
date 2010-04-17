#
# sample makefile
#

HOME       := /u/rvdg

# indicate where the source files are
SRC_PATH   := .

# indicate where the object files are to be created
OBJ_PATH   := .

# indicate where the FLAME libraries reside
LIB_PATH   := $(HOME)/flame/lib

# indicate where the FLAME include files reside
INC_PATH   := $(HOME)/flame/include

# indicate the path to the BLAS libraries
BLAS_PATH  := $(HOME)/flame/lib

# indicate what BLAS library to use
BLAS_LIB   := $(BLAS_PATH)/libgoto.a

# indicate the FLAME base library
FLAME_BASE := $(LIB_PATH)/libflame.a

# indicate the FLAME BLAS library
FLAME_BLAS := $(LIB_PATH)/libflame-blas.a

# indicate the FLAME lapack library
FLAME_LAPACK := $(LIB_PATH)/libflame-lapack.a

CC         := gcc
FC         := gfortran
LINKER     := $(FC)
CFLAGS     := -I$(SRC_PATH) \
              -I$(INC_PATH) \
              -g -O3 -fomit-frame-pointer -funroll-all-loops \
              -finline-functions \
              -Wmissing-prototypes
FFLAGS     := $(CFLAGS)
LDFLAGS    := -lm

testmatrix: testmatrix.o
	$(CC) $(CFLAGS) $< -o $@ $(FLAME_BASE) $(BLAS_LIB)
	
testmatrix.o: testmatrix.c
	$(CC) $(CFLAGS) -c $< -o $@
		
clean:
	rm -f *.o *~ testmatrix

