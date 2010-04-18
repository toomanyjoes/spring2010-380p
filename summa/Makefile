CHPLFLAGS = -O --fast
CHPL=chpl

summa: summa.chpl
	${CHPL} ${CHPLFLAGS} -o $@ summa.chpl

clean:
	rm -f *~ summa summa_real

.PHONY: clean
