#include "libDCF.h"

// Hidden terminals using CSMA/CA
B1simulation();

int main() 
{
	B1simulation();
}

void B1simulation() 
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
	// 		[0, 2^k*CW_0 - 1]. CW can not exceed CWmax
}
