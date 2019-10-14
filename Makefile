LIBS := -L./lib

lib/libDCF.so: src/libDCF.c
	 $(CC) -shared $^ -o  $@ -lm

build/DCFTester:src/test_libDCF.c
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

build/A1sim: src/A1sim.c lib/libDCF.so
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

build/A2sim: src/A2sim.c
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

build/B1sim: src/B1sim.c
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

build/B2sim: src/B2sim.c
	$(CC) $(LIBS) $^ -o $@ -lm -lDCF

build/script_runner: build/A1sim build/A2sim build/B1sim build/B2sim
	$(CC) $^ -o $@ $(LIBS) -lm

run_all_scripts: build/script_runner
	./script_runner