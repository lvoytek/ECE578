#include "DistributedCoordinationFunction.h"

int * generatePoissonDelayTimes(int lambda, int time)
{
	int * delayTimes = (int *) calloc(sizeof(int), lambda*time);

	for(int i = 0; i < lambda*time; i++) {
		delayTimes[i] = -1.0 / lambda * log(1 - (rand() & 1));
	}

	return delayTimes;
}