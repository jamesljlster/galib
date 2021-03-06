#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>

#include <galib.h>

#define CHRO_LEN		8
#define CHRO_RESERVE	20
#define ITER_COUNT		1000

double fitness(GA_TYPE* chro, int chroLen, void* arg);

int ga_print_chro(struct GA_POOL* gaPoolPtr, int chroIndex)
{
	int i;
	
	// Checking
	if(chroIndex >= gaPoolPtr->poolSize)
		return -1;
	
	for(i = 0; i < gaPoolPtr->chroLen; i++)
	{
		printf("%lf ", gaPoolPtr->pool[chroIndex][i]);
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int i;
	int iResult;
	int randIndex;
	int mutPosition;
	int mutValue;
	struct GA_POOL gaPool;
	double fitLog[ITER_COUNT] = {0};
	GA_TYPE chros[2][CHRO_LEN] = {0};
	FILE* fileWrite;

	srand(time(NULL));

	iResult = ga_create(&gaPool, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_create() failed!\n");
		return -1;
	}
		
	iResult = ga_insert(&gaPool, chros[0], CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_insert() failed!\n");
		return -1;
	}

	iResult = ga_insert(&gaPool, chros[1], CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_insert() failed!\n");
		return -1;
	}

	i = 0;
	while(i < ITER_COUNT)
	{
		if(kbhit())
		{
			if(getch() == 27)
				break;
		}
	
		// Mutation
		if(i % 100 == 0)
		{
			randIndex = rand() % gaPool.poolSize;
			mutPosition = rand() % CHRO_LEN;
			mutValue = (gaPool.pool[randIndex][mutPosition] == 0) ? 1 : 0;
			ga_edit_chro(&gaPool, randIndex, mutPosition, mutValue);
			//ga_mutation(&gaPool, rand() % gaPool.poolSize, rand() % CHRO_LEN);
		}

		// Crossover
		ga_crossover(&gaPool, 0, 1, rand() % (CHRO_LEN - 1));

		// Order
		ga_order(&gaPool, fitness, 0, NULL);

		// Print 1st fitness
		fitLog[i] = fitness(gaPool.pool[0], gaPool.chroLen, NULL);
		ga_print_chro(&gaPool, 0);
		printf(", fitness = %lf\n", fitLog[i]);

		// Kill 100+ chromosome
		ga_kill_after(&gaPool, 99);

		i++;
	}

	printf("\n");
	printf("All chromosome:\n");
	for(i = 0; i < gaPool.poolSize; i++)
	{
		ga_print_chro(&gaPool, i);
		printf(", fitness = %lf\n", fitness(gaPool.pool[i], gaPool.chroLen, NULL));
	}
	
	if(argc >= 2)
	{
		fileWrite = fopen(argv[1], "w");
		if(fileWrite != NULL)
		{
			fprintf(fileWrite, "# Fitness log\n");
			for(i = 0; i < ITER_COUNT; i++)
			{
				fprintf(fileWrite, "%lf\n", fitLog[i]);
			}

			fclose(fileWrite);
		}
	}

	ga_delete(&gaPool);

	return 0;
}

double fitness(GA_TYPE* chro, int chroLen, void* arg)
{
	int i;
	int calcTmp;
	int result;
	
	result = 0;
	calcTmp = 1;
	for(i = 0; i < chroLen; i++)
	{
		result += (chro[i]) * calcTmp;
		calcTmp *= 2;
	}

	return (double)(255 - abs(170 - result));
}
