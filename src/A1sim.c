#include "libDCF.h"

// Concurrent Communications using CSMA/CA
void A1simulation();
void A1_sim_run(node *A, node *C);

const char *outputFileName = "A1simout.csv";


int main()
{
	A1simulation();
	return 0;
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
	float fairnessIndex[10] = {};

	printf("%sA1sim%s\n", BRIGHTMAGENTA, NONE);
	for (int i = 0; i < 10; i++) {
		initializeNode(&A);
		A.sendDelayTimes = generatePoissonDelayTimes(lambdaA[i], SIMULATION_TIME_S, SLOTS_PER_SECOND);

		initializeNode(&C);
		C.sendDelayTimes = generatePoissonDelayTimes(lambdaC[i], SIMULATION_TIME_S, SLOTS_PER_SECOND);

		A1_sim_run(&A, &C);

        	Acollisions[i] = A.totalCollisions;
        	Asuccesses[i] = A.totalSuccesses;
        	Ccollisions[i] = C.totalCollisions;
        	Csuccesses[i] = C.totalSuccesses;
        	fairnessIndex[i] = ((float)(A.slotsOccupied))/C.slotsOccupied;

		printf("%sA1sim: lambdaA: %d lambdaC: %d %s\n", YELLOW, lambdaA[i], lambdaC[i], NONE);
		printf("\tA successes: %d\n", A.totalSuccesses);
		printf("\tA collisions: %d\n", A.totalCollisions);
		printf("\tC successes: %d\n", C.totalSuccesses);
		printf("\tC collisions: %d\n\n", C.totalCollisions);

		printf("\tTotal Data Transferred: %d\n", (A.totalSuccesses + C.totalSuccesses) * DATA_FRAME_SIZE_BYTES);
        	printf("\tTotal Collisions: %d\n", A.totalCollisions + C.totalCollisions);
        	printf("\tSlots occupied by A:%d\n", A.slotsOccupied);
        	printf("\tSlots occupied by C:%d\n", C.slotsOccupied);
        	printf("\tFairness index: %f\n\n", fairnessIndex[i]);
		free(A.sendDelayTimes);
		free(C.sendDelayTimes);
	}

	sendToCSV(outputFileName, lambdaA, lambdaC, Asuccesses, Csuccesses, Acollisions, Ccollisions, fairnessIndex);
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
			A->slotsOccupied += NAV;
			C->slotsOccupied += NAV;

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
				i += NAV;
				i += DIFS_SLOTS;
                A->slotsOccupied += NAV;

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
				i += NAV;
				i += DIFS_SLOTS;
                A->slotsOccupied += NAV;

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
				i += NAV;
				i += DIFS_SLOTS;
				C->slotsOccupied += NAV;

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
				i += NAV;
				i += DIFS_SLOTS;
				C->slotsOccupied += NAV;

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
