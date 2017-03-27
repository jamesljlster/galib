#include <stdlib.h>
#include <stdio.h>
#include <galib.h>

#define CHRO_LEN	16
#define SET	3

int main()
{
	int i;
	int iResult;
	struct GA_POOL ga;

	double chro[CHRO_LEN] = {0};

	// Create ga pool
	iResult = ga_create(&ga, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_create() failed!");
		return -1;
	}

	// Insert chromosome
	for(i = 0; i < SET; i++)
	{
		iResult = ga_insert(&ga, chro, CHRO_LEN);
		if(iResult < 0)
		{
			printf("ga_insert() failed!");
			return -1;
		}
	}

	// Remove test
	for(i = 0; i < SET; i++)
	{
		iResult = ga_remove(&ga, 0);
		if(iResult < 0)
		{
			printf("ga_remove() failed!");
			return -1;
		}
	}

	// Print ga pool
	printf("Pool size: %d\n", ga.poolSize);
	printf("Pool addr: %p\n", ga.pool);

	return 0;
}
