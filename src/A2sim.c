#include "libDCF.h"

// Concurrent Communications using CSMA/CA with virtual carrier 
// sensing enabled
A2simulation();

int main()
{
	A2simulation();
}

void A2simulation()
{
	//RTS and CTS exchanged

	// if RTS collision occurs, stations that collided doubles
	//		their contention window and repeats the backoff process
	//		after k collisions, backoff value is selected from
	// 		[0, 2^k*CW_0 - 1]. CW can not exceed CWmax

	// otherwise, overhearing stations defer from transmission
	//		for time indicated in NAV vector
}
