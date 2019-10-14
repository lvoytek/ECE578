build/DCFTester:src/libDCF.c src/test_libDCF.c
	$(CC) $^ -o $@ -lm

build/A1sim: src/A1sim.c
	$(CC) $^ -o $@ -lm

build/A2sim: src/A2sim.c
	$(CC) $^ -o $@ -lm

build/B1sim: src/B1sim.c
	$(CC) $^ -o $@ -lm

build/B2sim: src/B2sim.c
	$(CC) $^ -o $@ -lm

build/script_runner: build/A1sim build/A2sim build/B1sim build/B2sim
	$(CC) $^ -o $@ -lm

run_all_scripts: build/script_runner
	./script_runner