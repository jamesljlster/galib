#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <galib.h>

#define TAYLOR_CONST	1
#define TAYLOR_ARG		0.5
#define TAYLOR_SERIES	20

#define RAND_PRECISION	100000
#define RAND_NUM_MAX	10
#define RAND_NUM_MIN	-10
#define RAND_EXP_MAX	20
#define RAND_EXP_MIN	-20

#define MUT_RATE	0.4
#define GA_RESERVE	50
#define CHRO_LEN	20

void rand_chro(GA_TYPE* chro, int chroLen);
double rand_node();
double fitness(GA_TYPE* chro, int chroLen, void* arg);
double factorial(int series);
double taylor_exp(double x, double a, double numOfSeries);

int main(int argc, char* argv)
{
	int i, j;
	int iResult;

	int crossIndex;

	GA_TYPE tmpChros[2][CHRO_LEN] = {0};
	struct GA_POOL ga;

	srand(time(NULL));

	// Create ga pool
	iResult = ga_create(&ga, CHRO_LEN);
	if(iResult < 0)
	{
		printf("ga_create() failed!\n");
		return -1;
	}

	// Generate random chrosomes
	for(i = 0; i < 2; i++)
	{
		rand_chro(tmpChros[i], CHRO_LEN);
	}

	// Insert chrosomes to ga pool
	for(i = 0; i < 2; i++)
	{
		iResult = ga_insert(&ga, tmpChros[i], CHRO_LEN);
		if(iResult < 0)
		{
			printf("ga_insert() failed!\n");
			return -1;
		}
	}
	
	while(1)
	{
		if(kbhit())
		{
			if(getch() == 27)
				break;
		}

		// Crossover
		crossIndex = ga_crossover(&ga, 0, 1, rand() % (CHRO_LEN - 1));
		if(crossIndex < 0)
		{
			printf("ga_crossover failed!\n");
			return -1;
		}

		// Reproduction
		for(i = 0; i < 4; i++)
		{
			iResult = ga_reproduction(&ga, i + crossIndex);
			if(iResult < 0)
			{
				printf("ga_reproduction failed!\n");
				return -1;
			}
		}

		// Mutation
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < CHRO_LEN; j++)
			{
				if(rand() % 100 < MUT_RATE * 100)
				{
					ga_edit_chro(&ga, i + crossIndex, j, rand_node());
				}
			}
		}

		// Order
		ga_order(&ga, fitness, 1, NULL);

		// Print 1st fitness
		printf("1st fitness: %lf\n", fitness(ga.pool[0], CHRO_LEN, NULL));

		// Kill
		ga_kill_after(&ga, GA_RESERVE);
	}

	return 0;
}

double calc_chro(GA_TYPE* chro, int chroLen, double x)
{
	int i;
	double powTmp;
	double calcTmp;
	
	calcTmp = 0;
	for(i = 0; i < chroLen; i++)
	{
		calcTmp += chro[i] * pow(i, x);
	}

	return calcTmp;
}

double fitness(GA_TYPE* chro, int chroLen, void* arg)
{
	return fabs(taylor_exp(TAYLOR_ARG, TAYLOR_CONST, chroLen) - calc_chro(chro, chroLen, TAYLOR_ARG));
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
	double result;

	randRange = RAND_NUM_MAX - RAND_NUM_MIN + 1;
	randRange *= RAND_PRECISION;
	
	randExp = rand() % (RAND_EXP_MAX - RAND_EXP_MIN + 1) + RAND_EXP_MIN;
	randNum =  (double)(rand() % randRange) / (RAND_PRECISION) + RAND_NUM_MIN;

	return pow(randNum, randExp);
}

double taylor_exp(double x, double a, double numOfSeries)
{
	int i;
	double calcTmp;

	calcTmp = 0;
	for(i = 0; i < numOfSeries; i++)
	{
		calcTmp += 1.0 / factorial(i) * exp(a) * pow(x - a, i);
	}

	return calcTmp;
}

double factorial(int series)
{
	int i;
	double calcTmp;

	calcTmp = 1;
	for(i = 2; i <= series; i++)
	{
		calcTmp *= (double)i;
	}

	return calcTmp;
}
