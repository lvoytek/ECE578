#include "DistributedCoordinationFunctionTester.h"

void testPoisson()
{
	float * items = generatePoissonDelayTimes(50, 10);

	printf("%f", items[0]);
	
	for(int i = 1; i < 500; i++) {
		printf(", %f", items[i]);
	}
}

int main()
{
	testPoisson();
	return 0;
}