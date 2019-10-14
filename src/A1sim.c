#include "libDCF.h"

// Concurrent Communications using CSMA/CA
void A1simulation();
void A1_sim_run(node *A, node *C);
void startTransmission();
void stopTransmission();

int currentlyOccupied = FALSE;


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


	for (int i = 0; i < 10; i++) {
		A.sendDelayTimes = generatePoissonDelayTimes(lambdaA[i], simulation_time_s, 100000);
		A.k = 0;
		A.totalCollisions = 0;
		A.totalSuccesses = 0;
		A.backlogFrames = 0;
		A.countdown = -1;

		C.sendDelayTimes = generatePoissonDelayTimes(lambdaC[i], simulation_time_s, 100000);
		C.k = 0;
		C.totalCollisions = 0;
		C.totalSuccesses = 0;
		C.backlogFrames = 0;
		C.countdown = -1;

		A1_sim_run(&A, &C);

		free(A.sendDelayTimes);
		free(C.sendDelayTimes);
	}
}

void A1_sim_run(node * A, node * C)
{
	slot curr_slot = {FALSE};
	slot next_slot = {FALSE};

	int *currAback = A->sendDelayTimes;
	int *currCback = C->sendDelayTimes;

	int i = 0;
	while(i < total_slots) {

		//First collision prior to backoffs
		if ((*currAback == 0) && (*currCback == 0)) {
			i += 1 + NAV;

			int windowMax = pow(2, A->k) * CWo;
			A->countdown = rand()%(windowMax);

			if(windowMax * 2 < CWmax)
				A->k++;

			windowMax = pow(2, C->k) * CWo;
			C->countdown = rand()%(windowMax);

			if(windowMax * 2 < CWmax)
				C->k++;

			i += DIFS_slots;

			A->totalCollisions++;
			C->totalCollisions++;

			currAback++;
			currCback++;

			A->backlogFrames++;
			C->backlogFrames++;
		}

		//Subsequent collisions
		if(A->countdown == 0 && C->countdown == 0)
		{


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
				i += DIFS_slots;

				A->totalSuccesses++;
				currAback++;

				A->countdown = -1;
				A->backlogFrames--;
			}

			//Transmit normally
			else if (*currAback == 0) {
				startTransmission();
				i += NAV;
				i += DIFS_slots;

				A->totalSuccesses++;
				currAback++;

			}

			//Backoff countdown decrement
			if(C->countdown > 0) {
				C->countdown --;
			}

			//Transmit after backoff
			else if(C->countdown == 0) {
				startTransmission();
				i += NAV;
				i += DIFS_slots;

				C->totalSuccesses++;
				currAback++;

				C->countdown = -1;
				C->backlogFrames--;
			}

			//Transmit normally
			else if (*currCback == 0){
				startTransmission();
				i += NAV;
				i += DIFS_slots;

				C->totalSuccesses++;
				currCback++;
			}
		}

		(*currAback)--;
		(*currCback)--;

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
