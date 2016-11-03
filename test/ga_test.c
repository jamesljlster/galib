#include <stdio.h>

#include <galib.h>

#define CHRO_LEN	8

int main()
{
	int iResult;
	struct GA_POOL gaPool;

	iResult = ga_create(&gaPool, CHRO_LEN);
	if(iResult != 0)
	{
		printf("ga_create() failed!\n");
		return -1;
	}
	
	iResult = ga_insert(&gaPool, "00101101", CHRO_LEN);
	if(iResult != 0)
	{
		printf("ga_insert() failed!\n");
		return -1;
	}
	
	iResult = ga_insert(&gaPool, "10110010", CHRO_LEN);
	if(iResult != 0)
	{
		printf("ga_insert() failed!\n");
		return -1;
	}

	ga_print_chro(&gaPool, 0);
	ga_print_chro(&gaPool, 1);

	ga_delete(&gaPool);

	return 0;
}
