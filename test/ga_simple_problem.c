#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <time.h>

#include <galib.h>

#define CHRO_LEN		8
#define CHRO_RESERVE	20

double fitness(char* chro, int chroLen);

int main()
{
	int i;
	int iResult;
	struct GA_POOL gaPool;

	srand(time(NULL));

	iResult = ga_create(&gaPool, CHRO_LEN);
	if(iResult != 0)
	{
		printf("ga_create() failed!\n");
		return -1;
	}
		
	iResult = ga_insert(&gaPool, "00000000", CHRO_LEN);
	if(iResult != 0)
	{
		printf("ga_insert() failed!\n");
		return -1;
	}

	iResult = ga_insert(&gaPool, "00000000", CHRO_LEN);
	if(iResult != 0)
	{
		printf("ga_insert() failed!\n");
		return -1;
	}

	i = 0;
	while(1)
	{
		if(kbhit())
		{
			if(getch() == 27)
				break;
		}
	
		// Mutation
		if(i == 100)
		{
			//ga_mutation(&gaPool, rand() % gaPool.poolSize, rand() % CHRO_LEN);
			ga_mutation(&gaPool, 0, rand() % CHRO_LEN);
			i = 0;
		}

		// Crossover
		ga_crossover(&gaPool, 0, 1, rand() % (CHRO_LEN - 1));

		// Order
		ga_order(&gaPool, fitness);

		// Print 1st fitness
		ga_print_chro(&gaPool, 0);
		printf(", fitness = %lf\n", fitness(gaPool.pool[0], gaPool.chroLen));

		// Kill 100+ chromosome
		ga_kill_after(&gaPool, 99);

		i++;
	}

	printf("\n");
	printf("All chromosome:\n");
	for(i = 0; i < gaPool.poolSize; i++)
	{
		ga_print_chro(&gaPool, i);
		printf(", fitness = %lf\n", fitness(gaPool.pool[i], gaPool.chroLen));
	}

	ga_delete(&gaPool);

	return 0;
}

double fitness(char* chro, int chroLen)
{
	int i;
	int calcTmp;
	int result;
	
	result = 0;
	calcTmp = 1;
	for(i = 0; i < chroLen; i++)
	{
		result += (chro[i] - '0') * calcTmp;
		calcTmp *= 2;
	}

	return (double)(255 - abs(170 - result));
}
