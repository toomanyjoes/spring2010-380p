COMSRC :=\
	array_funcs.c \
	parallel_prefix.c
PSRC := presum.c
PGSRC := presumGen.c
PCSRC := presumCompare.c
LOSRC := lastoccur.c
LOGSRC := lastoccurGen.c
TSSRC := teamStandings.c
TSGSRC := teamStandingsGen.c

COMOBJS := $(patsubst %.C, %.o, ${COMSRC} )
POBJS := $(patsubst %.C, %.o, ${PSRC} )
PGOBJS := $(patsubst %.C, %.o, ${PGSRC} )
PCOBJS := $(patsubst %.C, %.o, ${PCSRC} )
LOOBJS := $(patsubst %.C, %.o, ${LOSRC} )
LOGOBJS := $(patsubst %.C, %.o, ${LOGSRC} )
TSOBJS := $(patsubst %.C, %.o, ${TSSRC} )
TSGOBJS := $(patsubst %.C, %.o, ${TSGSRC} )
MPICCFLAGS := -fast -tp barcelona-64
MPICC=mpicc
CC=cc

all: presum presumGen presumCompare lastoccur lastoccurGen teamStandings teamStandingsGen

presum: ${COMOBJS} ${POBJS}
	${MPICC} ${MPICCFLAGS} ${COMOBJS} ${POBJS} -o presum

presumGen: ${PGOBJS}
	${CC} ${PGOBJS} -o presumGen

presumCompare: ${COMOBJS} ${PCOBJS}
	${MPICC} ${MPICCFLAGS} ${PCOBJS} array_funcs.c -o presumCompare

lastoccur: ${COMOBJS} ${LOOBJS}
	${MPICC} ${MPICCFLAGS} ${COMOBJS} ${LOOBJS} -o lastoccur

lastoccurGen: ${LOGOBJS}
	${CC} ${LOGOBJS} -o lastoccurGen

teamStandings: ${COMOBJS} ${TSOBJS}
	${MPICC} ${MPICCFLAGS} ${COMOBJS} ${TSOBJS} -o teamStandings

teamStandingsGen: ${TSGOBJS}
	${CC} ${TSGOBJS} -o teamStandingsGen

clean:
	rm -f *.o *~ presum presumGen lastoccur lastoccurGen teamStandings teamStandingsGen presumCompare

.PHONY: clean
