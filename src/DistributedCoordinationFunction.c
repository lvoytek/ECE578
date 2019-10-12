#include "DistributedCoordinationFunction.h"

float * generatePoissonDelayTimes(int lambda, int time)
{
	float * delayTimes = (float *) calloc(sizeof(float), lambda*time);

	for(int i = 0; i < lambda*time; i++) {
		delayTimes[i] = -1.0 / lambda * log(1 - (rand()%100 / 100.0));
	}

	return delayTimes;
}