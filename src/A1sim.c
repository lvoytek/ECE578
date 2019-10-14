#include "libDCF.h"

// Concurrent Communications using CSMA/CA
void A1simulation();
void A1_sim_run(int *Abackoff, int *Cbackoff);
void startTransmission();
void stopTransmission();

int currentlyOccupied = FALSE;


int main() 
{
	A1simulation();
}

void A1simulation() 
{
	int *AbackoffTimes;
	int *CbackoffTimes;

	int lambdaA[] = {50, 100, 200, 300, 500, 100, 200, 400, 600, 1000};
	int lambdaC[] = {50, 100, 200, 300, 500, 50, 100, 200, 300, 500};

	for (int i = 0; i < 10; i++) {
		AbackoffTimes = generatePoissonDelayTimes(lambdaA[i], simulation_time_s, 100000);
		CbackoffTimes = generatePoissonDelayTimes(lambdaC[i], simulation_time_s, 100000);
		A1_sim_run(AbackoffTimes, CbackoffTimes);
		free(AbackoffTimes);
		free(CbackoffTimes);
	}
}

void A1_sim_run(int *Abackoff, int *Cbackoff)
{
	slot curr_slot = {FALSE};
	slot next_slot = {FALSE};

	int *currAback = Abackoff;
	int *currCback = Cbackoff;

	//NAV = Tansmission time + SIFS + ACK
	int NAV = slots_from_bytes(data_frame_size_bytes) + \
		SIFS_duration_us/slot_duration_us + \
		slots_from_bytes(ACK_RTS_CTS_size_bytes);


	for(int i = 0; i < total_slots; i++) {

		if (curr_slot.occupied == FALSE) {
			if (*currAback == 0)
				startTransmission("A");
			if (*currCback == 0)
				startTransmission("C");
		}

		// if busy, monitors until idle. 
		//		If idle, Tx decrements with every idle slot.
		// 		if busy, Tx freezes its backoff counter.
		//		when counter reaches zero, Tx transmits its frame.


		// if frame is successfully received by Rx, Rx replies with 
		// 		ACK after SIFS time. 


		// if collision occurs, stations that collided doubles their 
		// 		contention window and repeats the backoff process
		//		after k collisions, backoff value is selected from
		// 		[0, 2^k*CW_0 - 1]. CW can not exceed CWmax
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