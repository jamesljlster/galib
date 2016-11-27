#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <galib.h>

#define TAYLOR_CONST	1
#define TAYLOR_SERIES	20

#define RAND_PRECISION	100000
#define RAND_NUM_MAX	10
#define RAND_NUM_MIN	-10
#define RAND_EXP_MAX	20
#define RAND_EXP_MIN	-20

void rand_chro(GA_TYPE* chro, int chroLen);
double rand_node();
double fitness(GA_TYPE* chro, int chroLen);
double factorial(int series);
double taylor_exp(double x, double a, double numOfSeries);

int main(int argc, char* argv)
{
	srand(time(NULL));
	printf("rand: %g\n", rand_node());

	return 0;
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
