COMSRC :=\
	array_funcs.c \
	parallel_prefix.c
PSRC := presum.c
LOSRC := lastoccur.c
LOGSRC := lastoccurGen.c
TSSRC := teamStandings.c
TSGSRC := teamStandingsGen.c

COMOBJS := $(patsubst %.C, %.o, ${COMSRC} )
POBJS := $(patsubst %.C, %.o, ${PSRC} )
LOOBJS := $(patsubst %.C, %.o, ${LOSRC} )
LOGOBJS := $(patsubst %.C, %.o, ${LOGSRC} )
TSOBJS := $(patsubst %.C, %.o, ${TSSRC} )
TSGOBJS := $(patsubst %.C, %.o, ${TSGSRC} )
CCFLAGS := -fast -tp barcelona-64
CC=mpicc

all: presum lastoccur lastoccurGen teamStandings teamStandingsGen

presum: ${COMOBJS} ${POBJS}
	${CC} ${CCFLAGS} ${COMOBJS} ${POBJS} -o presum

lastoccur: ${COMOBJS} ${LOOBJS}
	${CC} ${CCFLAGS} ${COMOBJS} ${LOOBJS} -o lastoccur

lastoccurGen: ${LOGOBJS}
	${CC} ${CCFLAGS} ${LOGOBJS} -o lastoccurGen

teamStandings: ${COMOBJS} ${TSOBJS}
	${CC} ${CCFLAGS} ${COMOBJS} ${TSOBJS} -o teamStandings

teamStandingsGen: ${TSGOBJS}
	${CC} ${CCFLAGS} ${TSGOBJS} -o teamStandingsGen

clean:
	rm -f *.o *~ presum lastoccur lastoccurGen teamStandings teamStandingsGen

.PHONY: clean
