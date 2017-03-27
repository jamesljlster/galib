#include <stdlib.h>
#include <stdio.h>
#include <galib.h>

#define CHRO_LEN	7
#define SET	3

void print_chro(GA_TYPE* chro, int chroLen);

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

	chro[6] = 1.0;
	iResult = ga_insert(&ga, chro, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_insert() failed!");
		return -1;
	}

	chro[6] = 2.0;
	iResult = ga_insert(&ga, chro, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_insert() failed!");
		return -1;
	}

	chro[6] = 0.0;
	iResult = ga_insert(&ga, chro, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_insert() failed!");
		return -1;
	}

	printf("Original pool: \n");
	printf("Pool size: %d\n", ga.poolSize);
	printf("Pool addr: %p\n", ga.pool);
	printf("\n");

	for(i = 0; i < ga.poolSize; i++)
	{
		print_chro(ga.pool[i], ga.chroLen);
	}
	
	printf("\n");

	// Remove same chromosome
	iResult = ga_remove_same_chro(&ga);
	if(iResult < 0)
	{
		printf("ga_remove_same_chro() failed!");
		return -1;
	}

	// Print ga pool
	printf("Reserved pool: \n");
	printf("Pool size: %d\n", ga.poolSize);
	printf("Pool addr: %p\n", ga.pool);
	printf("\n");

	for(i = 0; i < ga.poolSize; i++)
	{
		print_chro(ga.pool[i], ga.chroLen);
	}

	return 0;
}

void print_chro(GA_TYPE* chro, int chroLen)
{
	int i;

	for(i = 0; i < chroLen; i++)
	{
		printf("%lf ", chro[i]);
	}
	printf("\n");
}

