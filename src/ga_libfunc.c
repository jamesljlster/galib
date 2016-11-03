#include <stdlib.h>
#include <stdio.h>

#include "galib.h"

int ga_order(struct GA_POOL* gaPoolPtr, double (*fitness)(char* chro, int chroLen))
{
	int i, j;
	char* tmp;

	for(i = 0; i < gaPoolPtr->poolSize - 1; i++)
	{
		for(j = 0; j < gaPoolPtr->poolSize - i - 1; j++)
		{
			if(fitness(gaPoolPtr->pool[j], gaPoolPtr->chroLen) < fitness(gaPoolPtr->pool[j + 1], gaPoolPtr->chroLen))
			{
				tmp = gaPoolPtr->pool[j];
				gaPoolPtr->pool[j] = gaPoolPtr->pool[j + 1];
				gaPoolPtr->pool[j + 1] = tmp;
			}
		}
	}

	return 0;
}

int ga_print_chro(struct GA_POOL* gaPoolPtr, int chroIndex)
{
	int i;
	
	// Checking
	if(chroIndex >= gaPoolPtr->poolSize)
		return -1;
	
	for(i = 0; i < gaPoolPtr->chroLen; i++)
	{
		printf("%c", gaPoolPtr->pool[chroIndex][i]);
	}
	printf("\n");

	return 0;
}


int ga_insert(struct GA_POOL* gaPoolPtr, char* chro, int chroLen)
{
	int i;
	int retValue = 0;
	char* tmpChro = NULL;
	void* allocTmp = NULL;

	// Checking chromosome length
	if(gaPoolPtr->chroLen > 0 && gaPoolPtr->chroLen != chroLen)
		return -1;

	// Memory allocation
	tmpChro = malloc(sizeof(char) * chroLen);
	if(tmpChro == NULL)
	{
		retValue = -1;
		goto RET;
	}
	else
	{
		for(i = 0; i < chroLen; i++)
		{
			tmpChro[i] = chro[i];
		}
	}
	
	// Memory reallocation
	allocTmp = realloc(gaPoolPtr->pool, sizeof(char*) * (gaPoolPtr->poolSize + 1));
	if(allocTmp == NULL)
	{
		retValue = -1;
		goto RET;
	}
	else
	{
		gaPoolPtr->poolSize++;
		gaPoolPtr->pool = (char**)allocTmp;
		allocTmp = NULL;

		gaPoolPtr->pool[gaPoolPtr->poolSize - 1] = tmpChro;
		tmpChro = NULL;
	}

	goto RET;

RET:
	if(allocTmp != NULL)
		free(allocTmp);

	if(tmpChro != NULL)
		free(tmpChro);

	return retValue;
}

int ga_create(struct GA_POOL* gaPoolPtr, int chroLen)
{
	gaPoolPtr->chroLen = chroLen;
	gaPoolPtr->poolSize = 0;
	gaPoolPtr->pool = NULL;

	return 0;
}

int ga_delete(struct GA_POOL* gaPoolPtr)
{
	int i;

	for(i = 0; i < gaPoolPtr->poolSize; i++)
	{
		if(gaPoolPtr->pool[i] != NULL)
			free(gaPoolPtr->pool[i]);
	}
	if(gaPoolPtr->pool != NULL)
		free(gaPoolPtr->pool);

	return 0;
}
