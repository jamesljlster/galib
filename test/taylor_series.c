#include <stdio.h>
#include <math.h>
#include <galib.h>

#define TAYLOR_CONST	3
#define TAYLOR_SERIES	20

double factorial(int series);
double taylor_exp(double x, double a, double numOfSeries);

int main(int argc, char* argv)
{
	printf("exp: %lf\n", exp(TAYLOR_CONST));
	printf("taylor: %lf\n", taylor_exp(TAYLOR_CONST, TAYLOR_CONST, TAYLOR_SERIES));

	return 0;
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
