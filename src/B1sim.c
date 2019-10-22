#include "libDCF.h"

// Hidden terminals using CSMA/CA
void B1simulation();
void B1_sim_run(node *A, node *C);

const char *outputFileName = "B1simout.csv";


int main()
{
    B1simulation();
    return 0;
}

void B1simulation()
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

    printf("%sB1sim%s\n", BRIGHTMAGENTA, NONE);
    for (int i = 0; i < 10; i++) {
        initializeNode(&A);
        A.sendDelayTimes = generatePoissonDelayTimes(lambdaA[i], SIMULATION_TIME_S, SLOTS_PER_SECOND);

        initializeNode(&C);
        C.sendDelayTimes = generatePoissonDelayTimes(lambdaC[i], SIMULATION_TIME_S, SLOTS_PER_SECOND);

        A2_sim_run(&A, &C);

        Acollisions[i] = A.totalCollisions;
        Asuccesses[i] = A.totalSuccesses;
        Ccollisions[i] = C.totalCollisions;
        Csuccesses[i] = C.totalSuccesses;
        fairnessIndex[i] = ((float)(A.slotsOccupied))/C.slotsOccupied;

        printf("%sB1sim: lambdaA: %d lambdaC: %d %s\n", YELLOW, lambdaA[i], lambdaC[i], NONE);
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

void  B1_sim_run(node *A, node *C)
{
	// Random backoff time


	// if busy, monitors until idle. 
	//		If idle, Tx decrements with every idle slot.
	// 		if busy, Tx freezes its backoff counter.
	//		when counter reaches zero, Tx transmits its frame.


	// if frame is successfully received by Rx, Rx replies with 
	// 		ACK after SIFS time. 


	// if collision occurs, stations that collided doubles their 
	// 		contention window and repeats the backoff process
	//		after k collisions, backoff value is selected from
	// 		[0, 2^k*CW_0 - 1]. CW can not exceed CWMAX
}
