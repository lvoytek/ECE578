#include "libDCF.h"

int * generatePoissonDelayTimes(int lambda, int time, int slotsPerSecond)
{
	float * delayTimes = (float *) calloc(sizeof(float), lambda*time);

	for(int i = 0; i < lambda*time; i++) {
		delayTimes[i] = -1.0 / lambda * log(1 - (rand()%100 / 100.0));
	}

	int * delaySlots = (int *) calloc(sizeof(int), lambda*time);

	for(int i = 0; i < lambda*time; i++) {
		delaySlots[i] = delayTimes[i] * slotsPerSecond / time;
	}

	free(delayTimes);

	return delaySlots;
}

float slots_from_bytes(int bytes)
{
	return ((bytes * 8 ) / (TransmissionRate_Mbps*oneMillion))/(slot_duration_us*oneMillion);
}