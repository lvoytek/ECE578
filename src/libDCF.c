#include "libDCF.h"

int * generatePoissonDelayTimes(int lambda, int time, int slotsPerSecond)
{
	float * delayTimes = (float *) calloc(lambda*time, sizeof(float));

	for(int i = 0; i < lambda*time; i++) {
		delayTimes[i] = -1.0 / lambda * log(1 - (rand()%100 / 100.0));
	}

	int * delaySlots = (int *) calloc(lambda*time, sizeof(int));

	for(int i = 0; i < lambda*time; i++) {
		delaySlots[i] = delayTimes[i] * slotsPerSecond / time;
	}

	free(delayTimes);

	return delaySlots;
}

float slots_from_bytes(int bytes)
{
	return (((float)bytes * 8 ) / ((float)TransmissionRate_Mbps*(float)slot_duration_us));
}