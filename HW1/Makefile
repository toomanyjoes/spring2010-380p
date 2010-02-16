
all: barrier.o lastoccur lastoccurGen teamStandings teamStandingsGen presum

lastoccur: parallel_prefix array_funcs lastoccur.o
	gcc -lpthread -o lastoccur parallel_prefix.o lastoccur.o array_funcs.o barrier.o

lastoccurGen: lastoccurGen.c
	gcc -o lastoccurGen lastoccurGen.c

teamStandingsGen: teamStandingsGen.c
	gcc -o teamStandingsGen teamStandingsGen.c

teamStandings: parallel_prefix array_funcs teamStandings.o
	gcc -lpthread -o teamStandings parallel_prefix.o teamStandings.o  array_funcs.o barrier.o

presum: parallel_prefix array_funcs presum.o
	gcc -lpthread -o presum parallel_prefix.o presum.o  array_funcs.o barrier.o

parallel_prefix: parallel_prefix.c array_funcs
	gcc -g -c -pthread parallel_prefix.c 

array_funcs: array_funcs.c
	gcc -g  -c array_funcs.c

lastoccur.o: lastoccur.c
	gcc -g  -c lastoccur.c

teamStandings.o: teamStandings.c
	gcc -g  -c teamStandings.c

presum.o: presum.c
	gcc -g -c presum.c

barrier.o: barrier.c
	gcc -g -c barrier.c

clean:
	rm -rf *.o teamStandings presum lastoccur teamStandingsGen lastoccurGen
