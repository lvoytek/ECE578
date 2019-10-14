#ifndef DISTRIBUTEDCOORDINATIONFUNCTION_H
#define DISTRIBUTEDCOORDINATIONFUNCTION_H

#include <stdio.h> //printf
#include <stdlib.h> //calloc, rand
#include <math.h> //log

#define oneMillion 1000000

#define data_frame_size_bytes 1500
#define slot_duration_us 10
#define SIF_duration_us 10
#define CWmax 1024
#define CWo 4
#define ACK_RTS_CTS_size_bytes 30
#define DIFS_duration_us 40
#define TransmissionRate_Mbps 12
#define simulation_time_s 10
#define total_slots (simulation_time_s * 10000000) / slot_duration_us

#define FALSE 0
#define TRUE 1

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


#endif