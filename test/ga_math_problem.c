#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <galib.h>

#define CHRO_LEN	20
#define NODE_MAX	30
#define NODE_MIN	-30

#define MUT_RATE	0.4

#define GA_RESERVE	50
#define ITER_COUNT	10000

#define TARGET		1333

void RandChro(GA_TYPE* chro, int chroLen);
void Mutation(GA_TYPE* chro, int chroLen);
double fitness(GA_TYPE* chro, int chroLen);

int main(int argc, char* argv[])
{
	int i;
	int iResult;
	int iterCount;
	int crossIndex;

	double fitLog[ITER_COUNT] = {0};

	struct GA_POOL gaPool;

	GA_TYPE tmpChro[2][CHRO_LEN];

	FILE* fileWrite = NULL;

	// Create ga pool
	iResult = ga_create(&gaPool, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_create() failed\n");
		return -1;
	}

	// Generate random chromosome
	for(i = 0; i < 2; i++)
	{
		RandChro(tmpChro[i], CHRO_LEN);
	}

	// Insert chromosomes to ga pool
	for(i = 0; i < 2; i++)
	{
		iResult = ga_insert(&gaPool, tmpChro[i], CHRO_LEN);
		if(iResult < 0)
		{
			printf("ga_insert() failed\n");
			return -1;
		}
	}
	
	iterCount = 0;
	while(1 /*iterCount < ITER_COUNT*/)
	{
		if(kbhit())
		{
			if(getch() == 27)
				break;
		}

		// Crossover
		crossIndex = ga_crossover(&gaPool, 0, 1, rand() % (CHRO_LEN - 1));
		if(crossIndex < 0)
		{
			printf("ga_crossover() failed!\n");
			return -1;
		}

		// Reproduction
		for(i = 0; i < 4; i++)
		{
			iResult = ga_reproduction(&gaPool, i + crossIndex);
			if(iResult < 0)
			{
				printf("ga_reproduction failed\n");
				return -1;
			}
		}

		// Mutatuon
		for(i = 0; i < 4; i++)
		{
			if(rand() % 100 < MUT_RATE * 100)
				Mutation(gaPool.pool[i + crossIndex], CHRO_LEN);
		}
			
		// Order
		ga_order(&gaPool, fitness);

		// Print 1st fitness
		fitLog[iterCount] = fitness(gaPool.pool[0], CHRO_LEN);
		printf("1st fitness: %lf\n", fitLog[iterCount]);

		// Kill
		ga_kill_after(&gaPool, GA_RESERVE);

		iterCount++;
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
		}
		fclose(fileWrite);
	}

	return 0;
}

void Mutation(GA_TYPE* chro, int chroLen)
{
	int i;
	for(i = 0; i < chroLen; i++)
	{
		if(rand() % 100 < MUT_RATE * 100)
			chro[i] = rand() % (NODE_MAX - NODE_MIN + 1) + NODE_MIN;
	}
}

void RandChro(GA_TYPE* chro, int chroLen)
{
	int i;
	for(i = 0; i < chroLen; i++)
	{
		chro[i] = rand() % (NODE_MAX - NODE_MIN + 1) + NODE_MIN;
	}
}


double fitness(GA_TYPE* chro, int chroLen)
{
	int i;
	double calcTmp;
	
	calcTmp = chro[0];
	for(i = 0; i < CHRO_LEN - 1; i++)
	{
		calcTmp += pow(-1, i % 2) * chro[i + 1];
	}

	return TARGET - fabs(TARGET - calcTmp);
}
