#include "libDCF.h"

int * generatePoissonDelayTimes(int lambdaA, int lambdaC, int time, int slotsPerSecond)
{
	float * delayTimes = (float *) calloc(sizeof(float), lambdaA*time);

	for(int i = 0; i < lambdaA*time; i++) {
		delayTimes[i] = -1.0 / lambdaC * log(1 - (rand()%100 / 100.0));
	}

	int * delaySlots = (int *) calloc(sizeof(int), lambdaA*time);

	for(int i = 0; i < lambdaA*time; i++) {
		delaySlots[i] = delayTimes[i] * slotsPerSecond / time;
	}

	free(delayTimes);

	return delaySlots;
}