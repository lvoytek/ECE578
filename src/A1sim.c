#include "libDCF.h"

// Concurrent Communications using CSMA/CA
void A1simulation();
void A1_sim_run(node *A, node *C);
void startTransmission();
void stopTransmission();

int currentlyOccupied = FALSE;
const char *outputFileName = "A1simout.csv";


int main() 
{
	A1simulation();
}

void A1simulation() 
{
	node A;
	node C;

	int lambdaA[] = {50, 100, 200, 300, 500, 100, 200, 400, 600, 1000};
	int lambdaC[] = {50, 100, 200, 300, 500, 50, 100, 200, 300, 500};

    int Acollisions[10] = {};
    int Asuccesses[10] = {};
    int Csuccesses[10] = {};
    int Ccollisions[10] = {};

	printf("%sA1sim%s\n", BRIGHTMAGENTA, NONE);
	for (int i = 0; i < 10; i++) {
		A.sendDelayTimes = generatePoissonDelayTimes(lambdaA[i], SIMULATION_TIME_S, 100000);
		A.k = 0;
		A.totalCollisions = 0;
		A.totalSuccesses = 0;
		A.backlogFrames = 0;
		A.countdown = -1;

		C.sendDelayTimes = generatePoissonDelayTimes(lambdaC[i], SIMULATION_TIME_S, 100000);
		C.k = 0;
		C.totalCollisions = 0;
		C.totalSuccesses = 0;
		C.backlogFrames = 0;
		C.countdown = -1;

		A1_sim_run(&A, &C);

        Acollisions[i] = A.totalCollisions;
        Asuccesses[i] = A.totalSuccesses;
        Ccollisions[i] = C.totalCollisions;
        Csuccesses[i] = C.totalSuccesses;

		printf("%sA1sim: lambdaA: %d lambdaC: %d %s\n", YELLOW, lambdaA[i], lambdaC[i], NONE);
		printf("\tA successes: %d\n", A.totalSuccesses);
		printf("\tA collisions: %d\n", A.totalCollisions);
		printf("\tC successes: %d\n", C.totalSuccesses);
		printf("\tC collisions: %d\n\n", C.totalCollisions);

		free(A.sendDelayTimes);
		free(C.sendDelayTimes);
	}
	FILE *file = fopen(outputFileName, "w+");
	fprintf(file, "lambdaA,");
	for (int i = 0; i < 10; i++) {
        fprintf(file,"%d",lambdaA[i]);
        if (i != 9) fprintf(file, ",");
    }
	fprintf(file, "\nlambdaC,");
    for (int i = 0; i < 10; i++) {
        fprintf(file,"%d",lambdaC[i]);
        if (i != 9) fprintf(file, ",");
    }
    fprintf(file, "\nAcollisions,");
    for (int i = 0; i < 10; i++) {
        fprintf(file,"%d",Acollisions[i]);
        if (i != 9) fprintf(file, ",");
    }
    fprintf(file, "\nAsuccesses,");
    for (int i = 0; i < 10; i++) {
        fprintf(file,"%d",Asuccesses[i]);
        if (i != 9) fprintf(file, ",");
    }
    fprintf(file, "\nCcollisions,");
    for (int i = 0; i < 10; i++) {
        fprintf(file,"%d",Ccollisions[i]);
        if (i != 9) fprintf(file, ",");
    }
    fprintf(file, "\nCsuccesses,");
    for (int i = 0; i < 10; i++) {
        fprintf(file,"%d",Csuccesses[i]);
        if (i != 9) fprintf(file, ",");
    }
    fprintf(file, "\n");
    fclose(file);
}

void A1_sim_run(node * A, node * C)
{
	slot curr_slot = {FALSE};
	slot next_slot = {FALSE};

	int currAback = 0;
	int currCback = 0;

	int i = 0;
	while(i < TOTAL_SLOTS) {
		//First collision prior to backoffs
		if (((A->sendDelayTimes[currAback] == 0) && (C->sendDelayTimes[currCback] == 0)) || (A->countdown == 0 && C->countdown == 0)) {
			i += 1 + NAV;

			int windowMax = pow(2, A->k) * CWo;
			A->countdown = rand()%(windowMax);

			if(windowMax * 2 < CWMAX)
				A->k++;

			windowMax = pow(2, C->k) * CWo;
			C->countdown = rand()%(windowMax);

			if(windowMax * 2 < CWMAX)
				C->k++;

			i += DIFS_SLOTS;

			A->totalCollisions++;
			C->totalCollisions++;

			currAback++;
			currCback++;

			A->backlogFrames++;
			C->backlogFrames++;
		}

		else
		{
			//Backoff countdown decrement
			if(A->countdown > 0) {
				A->countdown --;
			}

			//Transmit after backoff
			else if(A->countdown == 0) {
				startTransmission();
				i += NAV;
				i += DIFS_SLOTS;

				A->totalSuccesses++;
				currAback++;

				A->countdown = -1;
				A->backlogFrames--;

				if(A->backlogFrames > 0)
					A->countdown = DIFS_SLOTS;
				else
					A->countdown = -1;

				A->k = 0;
			}

			//Transmit normally
			else if (A->sendDelayTimes[currAback] == 0) {
				startTransmission();
				i += NAV;
				i += DIFS_SLOTS;

				A->totalSuccesses++;
				currAback++;

				A->k=0;
			}

			//Wait for next arrivalfree(): invalid next size (normal)
			else {
				A->sendDelayTimes[currAback]--;
			}

			//Backoff countdown decrement
			if(C->countdown > 0) {
				C->countdown --;
			}

			//Transmit after backoff
			else if(C->countdown == 0) {
				startTransmission();
				i += NAV;
				i += DIFS_SLOTS;

				C->totalSuccesses++;
				currCback++;

				C->backlogFrames--;

				if(C->backlogFrames > 0)
					C->countdown = DIFS_SLOTS;
				else
					C->countdown = -1;

				C->k = 0;
			}

			//Transmit normally
			else if (C->sendDelayTimes[currCback] == 0){
				startTransmission();
				i += NAV;
				i += DIFS_SLOTS;

				C->totalSuccesses++;
				currCback++;

				C->k = 0;
			}

			//Wait for next arrival
			else {
				C->sendDelayTimes[currCback]--;
			}
		}
		i++;
	}
}

void startTransmission() 
{
	currentlyOccupied = TRUE;
}

void stopTransmission() 
{
	currentlyOccupied = FALSE;
}
