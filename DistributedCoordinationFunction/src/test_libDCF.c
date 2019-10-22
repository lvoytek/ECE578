#include "test_libDCF.h"

void testPoisson()
{
	int * items = generatePoissonDelayTimes(50, 10, 100000);

	printf("%d", items[0]);
	
	for(int i = 1; i < 500; i++) {
		printf(", %d", items[i]);
	}
}

int main()
{
	testPoisson();
	return 0;
}