#ifndef DISTRIBUTEDCOORDINATIONFUNCTION_H
#define DISTRIBUTEDCOORDINATIONFUNCTION_H

#include <stdio.h> //printf
#include <stdlib.h> //calloc, rand
#include <stdint.h> //uint8_t
#include <math.h> //log

#define oneMillion 1000000

#define DATA_FRAME_SIZE_BYTES 1500
#define SLOT_DURATION_us 10
#define SLOTS_PER_SECOND 100000
#define SIFS_DURATION_us 10
#define CWMAX 1024
#define CWo 4
#define ACK_RTS_CTS_SIZE_BYTES 30
#define DIFS_DURATION_us 40
#define TRANSMISSIONRATE_Mbps 12
#define SIMULATION_TIME_S 10
#define TOTAL_SLOTS (SIMULATION_TIME_S * 1000000) / SLOT_DURATION_us
#define DIFS_SLOTS DIFS_DURATION_us/SLOT_DURATION_us

#define FALSE 0
#define TRUE 1

#define YELLOW "\e[1;33m"
#define NONE "\e[1;0m"
#define BRIGHTMAGENTA "\e[1;35m"


/*
* Generates the amount of slots it takes to transmission x bytes.
*/
float slots_from_bytes(int bytes);

// NAV = data frame + SIFS + ACK
#define NAV (int)(slots_from_bytes(DATA_FRAME_SIZE_BYTES) + \
		(float)SIFS_DURATION_us/(float)SLOT_DURATION_us + \
		slots_from_bytes(ACK_RTS_CTS_SIZE_BYTES))


typedef struct slot_t
{
	uint8_t occupied;
	size_t * next;
} slot;


/*
* Represents a node attempting to send and receive packets
* at set times
*/
typedef struct node_t
{
	int * sendDelayTimes;
	int countdown;
	int backlogFrames;
	int totalCollisions;
	int totalSuccesses;
	int k;
	int framesOccupied;
} node;

/*
* Used as a container for linked lists of nodes
*/
typedef struct nodeListItem_t
{
	node * nodeValue;
	struct nodeListItem_t * next;

} nodeListItem;

/*
* Contains a set of nodes that are transmitting on the
* same medium
*/
typedef struct collisionDomain_t
{
	nodeListItem * domainNodes;
} collisionDomain;

/*
* Generate λ * # of seconds points as an array of integers
* representing the delay times in slots for sending frames
* over a network medium
*
* The equation used is:
* X = -1/λ * ln(1-U) where U is a set of 1's and 0's
*
* lambda is in frames/second and time is in seconds
*/
int * generatePoissonDelayTimes(int lambda, int time, int slotsPerSecond);

/*
 * Initialize a given node as not having sent anything
 */
void initializeNode(node * n);

#endif
