#include "libDCF.h"

int * generatePoissonDelayTimes(int lambda, int time, int slotsPerSecond)
{
	float * delayTimes = (float *) calloc(lambda*time*slotsPerSecond, sizeof(float));

	for(int i = 0; i < lambda*time; i++) {
		delayTimes[i] = -1.0 / lambda * log(1 - (rand()%100 / 100.0));
	}

	int * delaySlots = (int *) calloc(lambda*time*slotsPerSecond, sizeof(int));

	for(int i = 0; i < lambda*time; i++) {
		delaySlots[i] = delayTimes[i] * slotsPerSecond / time;
	}
	free(delayTimes);

	return delaySlots;
}

float slots_from_bytes(int bytes)
{
	return (((float)bytes * 8 ) / ((float)TRANSMISSIONRATE_Mbps * (float)SLOT_DURATION_us));
}

void initializeNode(node * n)
{
    n->k = 0;
    n->totalCollisions = 0;
    n->totalSuccesses = 0;
    n->slotsOccupied = 0;
    n->backlogFrames = 0;
    n->countdown = -1;
}

void sendToCSV(const char * filename, int lambdaA[], int lambdaC[], int successesA[], int successesC[],
               int collisionsA[], int collisionsC[], float fairnessIndex[])
{
    FILE *file = fopen(filename, "w+");

    fprintf(file, "lambda (frames/s),lambdaA (frames/s),lambdaC (frames/s),A Throughput (Kbps),C"
                  " Throughput(Kbps),A Collisions,C Collisions,Fairness Index\n");

    for(unsigned int i = 0; i < 10; i++) {
        fprintf(file,"%d,%d,%d,%.1f,%.1f,%d,%d,%f\n", lambdaC[i], lambdaA[i], lambdaC[i],
                successesA[i] * DATA_FRAME_SIZE_BYTES * 8 /((float) SIMULATION_TIME_S * 1000),
                successesC[i] * DATA_FRAME_SIZE_BYTES * 8 /((float) SIMULATION_TIME_S * 1000),
                collisionsA[i], collisionsC[i], fairnessIndex[i]);
    }

    fclose(file);
}