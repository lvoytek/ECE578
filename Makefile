LIBS ?= -L./lib
SRCDIR ?= src
BUILDDIR ?= build
LIBDIR ?= lib

.phony: clean A1 A2 B1 B2 all

all: A1 A2 B1 B2

A1: ${BUILDDIR}/A1sim
	./${BUILDDIR}/A1sim
A2: ${BUILDDIR}/A2sim
	./${BUILDDIR}/A2sim
B1: ${BUILDDIR}/B1sim
	./${BUILDDIR}/B1sim
B2: ${BUILDDIR}/B2sim
	./${BUILDDIR}/B2sim

${LIBDIR}/libDCF.so: ${SRCDIR}/libDCF.c
	 @mkdir -p ${LIBDIR}
	 $(CC) -shared $^ -o  $@ -lm

${BUILDDIR}/DCFTester:${SRCDIR}/test_libDCF.c
	@mkdir -p ${BUILDDIR}
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

${BUILDDIR}/A1sim: ${SRCDIR}/A1sim.c ${LIBDIR}/libDCF.so
	@mkdir -p ${BUILDDIR}
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

${BUILDDIR}/A2sim: ${SRCDIR}/A2sim.c ${LIBDIR}/libDCF.so
	@mkdir -p ${BUILDDIR}
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

${BUILDDIR}/B1sim: ${SRCDIR}/B1sim.c ${LIBDIR}/libDCF.so
	@mkdir -p ${BUILDDIR}
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

${BUILDDIR}/B2sim: ${SRCDIR}/B2sim.c ${LIBDIR}/libDCF.so
	@mkdir -p ${BUILDDIR}
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

clean:
	rm -rf build
	rm -rf lib
