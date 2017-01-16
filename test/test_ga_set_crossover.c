#include <stdio.h>
#include <galib.h>

#define CHRO_LEN	10

void print_chro(GA_TYPE* chro, int chroLen);

int main()
{
	int i;
	int iResult;

	struct GA_POOL ga;
	struct GA_SET gaSet;

	GA_TYPE chro1[CHRO_LEN] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 
	GA_TYPE chro2[CHRO_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

	// Create ga pool
	iResult = ga_create(&ga, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_create() failed!\n");
		return -1;
	}

	// Set ga set
	gaSet = ga_set_init(2, 5);

	// Insert chromosome to ga pool
	iResult = ga_insert(&ga, chro1, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_insert() failed!\n");
		return -1;
	}

	iResult = ga_insert(&ga, chro2, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_insert() failed!\n");
		return -1;
	}

	// Print current ga pool
	printf("Current ga pool:\n");
	for(i = 0; i < ga.poolSize; i++)
	{
		print_chro(ga.pool[i], CHRO_LEN);
	}
	printf("\n");

	// Processing ga set crossover
	iResult = ga_set_crossover(&ga, gaSet, 0, 1, 2);
	if(iResult < 0)
	{
		printf("ga_set_crossover() failed!\n");
		return -1;
	}
	else
	{
		printf("After ga_set_crossover with {2, 5} and cut index: 2\n");
		for(i = iResult; i < ga.poolSize; i++)
		{
			print_chro(ga.pool[i], CHRO_LEN);
		}
		printf("\n");
	}

	ga_delete(&ga);
	
	return 0;
}

void print_chro(GA_TYPE* chro, int chroLen)
{
	int i;

	for(i = 0; i < chroLen; i++)
	{
		printf("%.0lf  ", chro[i]);
	}
	printf("\n");
}
