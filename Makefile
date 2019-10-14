LIBS ?= -L./lib

.phony: clean A1 all

all: A1 A2

A1: build/A1sim
	./build/A1sim
A2: build/A2sim

lib/libDCF.so: src/libDCF.c
	 @mkdir -p lib
	 $(CC) -shared $^ -o  $@ -lm

build/DCFTester:src/test_libDCF.c
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

build/A1sim: src/A1sim.c lib/libDCF.so
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

build/A2sim: src/A2sim.c lib/libDCF.so
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

build/B1sim: src/B1sim.c lib/libDCF.so
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

build/B2sim: src/B2sim.c lib/libDCF.so
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

build/script_runner: build/A1sim build/A2sim build/B1sim build/B2sim lib/libDCF.so
	$(CC) $^ -o $@ $(LIBS) -lm

run_all_scripts: build/script_runner
	./script_runner

clean:
	rm -rf build/*
	rm -rf lib/*
