#include <stdio.h>
#include <math.h>

#include <galib.h>

#define CHRO_LEN	8

double fitness(char* chro, int chroLen);

int main()
{
	int i;
	int iResult;
	struct GA_POOL gaPool;

	iResult = ga_create(&gaPool, CHRO_LEN);
	if(iResult != 0)
	{
		printf("ga_create() failed!\n");
		return -1;
	}
		
	iResult = ga_insert(&gaPool, "11111111", CHRO_LEN);
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
		
	iResult = ga_insert(&gaPool, "01010101", CHRO_LEN);
	if(iResult != 0)
	{
		printf("ga_insert() failed!\n");
		return -1;
	}
	
	printf("order!\n");
	
	ga_order(&gaPool, fitness);
	
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
