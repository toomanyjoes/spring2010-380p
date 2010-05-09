CHPL=chpl
CHPLFLAGS = -O --fast

all: parallel

parallel: summa.chpl
	${CHPL} --fast -o summa_parallel summa.chpl

blksumma: psumma.chpl
	${CHPL} ${CHPLFLAGS} -o $@ psumma.chpl

serial: summa.chpl
	${CHPL} --fast --serial -o summa_serial summa.chpl

clean:
	rm -f *~ summa summa_real summa_parallel summa_parallel_real summa_serial summa_serial_real

.PHONY: clean
