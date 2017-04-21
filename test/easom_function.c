#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <galib.h>

#define RAND_PRECISION	10000
#define RAND_NUM_MAX	1
#define RAND_NUM_MIN	-1
#define RAND_EXP_MAX	1
#define RAND_EXP_MIN	-1

#define NUM_BASE	2.0

#define MUT_RATE	0.01
#define CHRO_LEN	12
#define GA_RESERVE	4

#define ITER_LIMIT	10000

double easom_func(double x, double y);
double fitness(GA_TYPE* chro, int chroLen, void* arg);
void rand_chro(GA_TYPE* chro, int chroLen);
double rand_node();

int main(int argc, char* argv[])
{
	int i, j;
	int iResult;
	int counter;
	int iterLimit;
	double mutRate;

	int crossIndex;
	GA_TYPE tmpChros[CHRO_LEN] = {0};
	struct GA_POOL ga;
	
	char* tmp;

	srand(time(NULL));
	
	// Set training argument
	mutRate = MUT_RATE;
	if(argc > 1)
	{
		mutRate = strtod(argv[1], &tmp);
		if(tmp == argv[1])
		{
			mutRate = MUT_RATE;
		}
	}
	
	iterLimit = ITER_LIMIT;
	if(argc > 2)
	{
		iterLimit = strtol(argv[2], &tmp, 10);
		if(tmp == argv[2])
		{
			iterLimit = ITER_LIMIT;
		}
	}

	// Create ga pool
	iResult = ga_create(&ga, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_create() failed!\n");
		return -1;
	}

	// Insert random chrosomes to ga pool
	for(i = 0; i < GA_RESERVE; i++)
	{
		rand_chro(tmpChros, CHRO_LEN);
		iResult = ga_insert(&ga, tmpChros, CHRO_LEN);
		if(iResult < 0)
		{
			printf("ga_insert() failed!\n");
			return -1;
		}
	}
	
	counter = 0;
	while(counter <= iterLimit)
	{
		counter++;
		
		// Crossover
		crossIndex = ga_crossover(&ga, 0, (rand() % (GA_RESERVE - 1)) + 1, rand() % (CHRO_LEN - 1));
		if(crossIndex < 0)
		{
			printf("ga_crossover failed!\n");
			return -1;
		}

		// Reproduction
		for(i = 0; i < GA_CROSS_OFFSPRING; i++)
		{
			iResult = ga_reproduction(&ga, i + crossIndex);
			if(iResult < 0)
			{
				printf("ga_reproduction failed!\n");
				return -1;
			}
		}

		// Mutation
		for(i = 0; i < GA_CROSS_OFFSPRING; i++)
		{
			for(j = 0; j < CHRO_LEN; j++)
			{
				if(rand() % 100 < mutRate * 100)
				{
					ga_edit_chro(&ga, i + crossIndex, j, rand_node());
				}
			}
		}

		// Kill same chromosome
		iResult = ga_remove_same_chro(&ga);
		if(iResult < 0)
		{
			printf("ga_remove_same_chro() failed!\n");
			return -1;
		}

		if(ga.poolSize < GA_RESERVE)
		{
			// Insert random chrosomes to ga pool
			for(i = ga.poolSize; i < GA_RESERVE; i++)
			{
				rand_chro(tmpChros, CHRO_LEN);
				iResult = ga_insert(&ga, tmpChros, CHRO_LEN);
				if(iResult < 0)
				{
					printf("ga_insert() failed!\n");
					return -1;
				}
			}
		}

		// Order
		ga_order(&ga, fitness, 1, NULL);

		// Kill
		ga_kill_after(&ga, GA_RESERVE);
	}
	
	// Print 1st fitness
	printf("%lf\n", fitness(ga.pool[0], CHRO_LEN, NULL));
	
	// Cleanup
	ga_delete(&ga);

	return 0;
}

double fitness(GA_TYPE* chro, int chroLen, void* arg)
{
	int i;
	int chroIndex;
	int halfLen;
	int quarterLen;
	double x, y;
	double calcTmp;
	
	// Find half and quarter of chromosome length
	halfLen = chroLen / 2;
	quarterLen = chroLen / 2;
	
	// Convert chromosome to x and y value
	x = 0;
	y = 0;
	
	chroIndex = 0;
	calcTmp = pow(NUM_BASE, quarterLen);
	for(i = 0; i < halfLen; i++)
	{
		x += chro[chroIndex++] * calcTmp;
		calcTmp /= NUM_BASE;
	}
	
	calcTmp = pow(NUM_BASE, quarterLen);
	for(i = 0; i < halfLen; i++)
	{
		y += chro[chroIndex++] * calcTmp;
		calcTmp /= NUM_BASE;
	}
	
	// Calculate function value
	return easom_func(x, y);;
}

double easom_func(double x, double y)
{
	return -cos(x) * cos(y) * exp(-(pow(x - M_PI, 2) + pow(y - M_PI, 2)));
}

void rand_chro(GA_TYPE* chro, int chroLen)
{
	int i;

	for(i = 0; i < CHRO_LEN; i++)
	{
		chro[i] = rand_node();
	}
}

double rand_node()
{
	int randRange;
	int randExp;
	double randNum;

	randRange = RAND_NUM_MAX - RAND_NUM_MIN + 1;
	randRange *= RAND_PRECISION;
	
	randExp = rand() % (RAND_EXP_MAX - RAND_EXP_MIN + 1) + RAND_EXP_MIN;
	randNum =  (double)(rand() % randRange) / (RAND_PRECISION) + RAND_NUM_MIN;

	return pow(randNum, randExp);
}
