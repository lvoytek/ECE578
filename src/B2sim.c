#include "libDCF.h"

// Hidden terminals using CSMA/CA with virtual carrier
void B2simulation();
void B2_sim_run(node *A, node *C);

const char *outputFileName = "B2simout.csv";


int main()
{
    B2simulation();
    return 0;
}

void B2simulation()
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

    printf("%sB2sim%s\n", BRIGHTMAGENTA, NONE);
    for (int i = 0; i < 10; i++) {
        initializeNode(&A);
        A.sendDelayTimes = generatePoissonDelayTimes(lambdaA[i], SIMULATION_TIME_S, SLOTS_PER_SECOND);

        initializeNode(&C);
        C.sendDelayTimes = generatePoissonDelayTimes(lambdaC[i], SIMULATION_TIME_S, SLOTS_PER_SECOND);

        B2_sim_run(&A, &C);

        Acollisions[i] = A.totalCollisions;
        Asuccesses[i] = A.totalSuccesses;
        Ccollisions[i] = C.totalCollisions;
        Csuccesses[i] = C.totalSuccesses;
        fairnessIndex[i] = ((float)(A.slotsOccupied))/C.slotsOccupied;

        printf("%sA2sim: lambdaA: %d lambdaC: %d %s\n", YELLOW, lambdaA[i], lambdaC[i], NONE);
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

void B2_sim_run(node *A, node *C)
{
	//RTS and CTS exchanged

	// if RTS collision occurs, stations that collided doubles
	//		their contention window and repeats the backoff process
	//		after k collisions, backoff value is selected from
	// 		[0, 2^k*CW_0 - 1]. CW can not exceed CWMAX

	// otherwise, overhearing stations defer from transmission
	//		for time indicated in NAV vector

    int ATransmissionCounter;
    int CTransmissionCounter;

    int ATransmitting = FALSE;
    int CTransmitting = FALSE;

    int ATransmissionIsRTS = TRUE;
    int CTransmissionIsRTS = TRUE;

    int ATransmissionFailure = FALSE;
    int CTransmissionFailure = FALSE;

    int currAback = 0;
    int currCback = 0;

    int i = 0;
    while(i < TOTAL_SLOTS) {

        //Backoff countdown decrement
        if(A->countdown > 0) {
            A->countdown --;
        }

        //Start transmission
        else if(A->countdown == 0 || A->sendDelayTimes[currAback] == 0) {
            ATransmissionCounter = RTS_CTS;
            ATransmitting = TRUE;
            ATransmissionFailure = FALSE;
            ATransmissionIsRTS = TRUE;

            A->countdown = -1;
            A->sendDelayTimes[currAback]--;

            if(A->backlogFrames > 0)
                A->backlogFrames--;

            //Collision from earlier C frame
            if(CTransmitting == TRUE)
            {
                ATransmissionFailure = TRUE;
                CTransmissionFailure = TRUE;
            }
        }

        //Wait for ACK
        else if(ATransmitting == TRUE) {

            if(ATransmissionCounter > 0)
                ATransmissionCounter--;

            //Designated Arrival Time
            else {
                ATransmitting = FALSE;

                //CTS Transmission Arrival
                if(ATransmissionIsRTS == TRUE) {
                    A->slotsOccupied += RTS_CTS;

                    if(ATransmissionFailure == TRUE) {
                        A->totalCollisions++;
                        A->backlogFrames++;

                        int windowMax = pow(2, A->k) * CWo;
                        A->countdown = rand()%(windowMax);

                        if(windowMax < CWMAX)
                            A->k++;
                    }

                    //Yay now transmit the real data
                    else {
                        ATransmissionCounter = NAV;
                        ATransmitting = TRUE;
                        ATransmissionFailure = FALSE;
                        ATransmissionIsRTS = FALSE;
                    }
                }

                //Data ACK Arrival
                else {
                    A->slotsOccupied += NAV;

                    if(ATransmissionFailure == TRUE) {
                        A->totalCollisions++;
                        A->backlogFrames++;

                        int windowMax = pow(2, A->k) * CWo;
                        A->countdown = rand()%(windowMax);

                        if(windowMax < CWMAX)
                            A->k++;
                    }

                    //Epic Win
                    else {
                        A->totalSuccesses++;
                        currAback++;

                        if(A->backlogFrames > 0)
                            A->countdown = DIFS_SLOTS;
                        else
                            A->countdown = -1;

                        A->k = 0;
                    }
                }
            }
        }

            //Wait for frame to hop on in this town
        else {
            A->sendDelayTimes[currAback]--;
        }


        //Backoff countdown decrement
        if(C->countdown > 0) {
            C->countdown --;
        }

        //Start transmission
        else if(C->countdown == 0 || C->sendDelayTimes[currCback] == 0) {
            CTransmissionCounter = RTS_CTS;
            CTransmitting = TRUE;
            CTransmissionFailure = FALSE;
            CTransmissionIsRTS = TRUE;

            C->countdown = -1;
            C->sendDelayTimes[currCback]--;

            if(C->backlogFrames > 0)
                C->backlogFrames--;

            //Collision from earlier C frame
            if(ATransmitting == TRUE)
            {
                ATransmissionFailure = TRUE;
                CTransmissionFailure = TRUE;
            }
        }

        //Wait for ACK
        else if(CTransmitting == TRUE) {

            if(CTransmissionCounter > 0)
                CTransmissionCounter--;

            //Designated Arrival Time
            else {
                CTransmitting = FALSE;

                //CTS Transmission Arrival
                if(CTransmissionIsRTS == TRUE) {
                    C->slotsOccupied += RTS_CTS;

                    if(CTransmissionFailure == TRUE) {
                        C->totalCollisions++;
                        C->backlogFrames++;

                        int windowMax = pow(2, C->k) * CWo;
                        C->countdown = rand()%(windowMax);

                        if(windowMax < CWMAX)
                            C->k++;
                    }

                    //Yay now transmit the real data
                    else {
                        CTransmissionCounter = NAV;
                        CTransmitting = TRUE;
                        CTransmissionFailure = FALSE;
                        CTransmissionIsRTS = FALSE;
                    }
                }

                //Data ACK Arrival
                else {
                    C->slotsOccupied += NAV;

                    if(CTransmissionFailure == TRUE) {
                        C->totalCollisions++;
                        C->backlogFrames++;

                        int windowMax = pow(2, C->k) * CWo;
                        C->countdown = rand()%(windowMax);

                        if(windowMax < CWMAX)
                            C->k++;
                    }

                    //Epic Win
                    else {
                        C->totalSuccesses++;
                        currCback++;

                        if(C->backlogFrames > 0)
                            C->countdown = DIFS_SLOTS;
                        else
                            C->countdown = -1;

                        C->k = 0;
                    }
                }
            }
        }

        //Wait for frame to hop on in this town
        else {
            C->sendDelayTimes[currCback]--;
        }

        i++;
    }
}
