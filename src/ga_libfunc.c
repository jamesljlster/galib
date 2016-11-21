#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "galib.h"
#include "debug.h"

void ga_copy(GA_TYPE* dst, GA_TYPE* src, int startIndex, int size);

int ga_kill_after(struct GA_POOL* gaPoolPtr, int killIndex)
{
	int i;
	int retValue = 0;
	void* allocTmp = NULL;
	
	// Checking
	LOG("Enter");
	LOG("killIndex: %d", killIndex);
	LOG("GA pool has %d chromosome", gaPoolPtr->poolSize);
	if(killIndex >= gaPoolPtr->poolSize)
	{
		LOG("Index out of range");
		retValue = -1;
		goto RET;
	}

	// Free chromosomes
	for(i = killIndex + 1; i < gaPoolPtr->poolSize; i++)
	{
		LOG("Free #%d chromosome", i);
		if(gaPoolPtr->pool[i] != NULL)
			free(gaPoolPtr->pool[i]);
	}

	allocTmp = realloc(gaPoolPtr->pool, sizeof(GA_TYPE*) * (killIndex + 1));
	if(allocTmp == NULL)
	{
		LOG("Realloc failed");
		retValue = -1;
		goto RET;
	}
	else
	{
		gaPoolPtr->poolSize = killIndex + 1;
		gaPoolPtr->pool = (GA_TYPE**)allocTmp;
		allocTmp = NULL;
		LOG("Reallocating memory succeed");
	}

RET:
	if(allocTmp != NULL)
		free(allocTmp);
	
	LOG("Exit");
	return retValue;
}

int ga_edit_chro(struct GA_POOL* gaPoolPtr, int chroIndex, int position, GA_TYPE newValue)
{
	LOG("Enter");
	LOG("chroIndex: %d, position: %d, newValue: %d", chroIndex, position, newValue);

	// Checking
	if(chroIndex >= gaPoolPtr->poolSize || position >= gaPoolPtr->chroLen)
		return -1;

	gaPoolPtr->pool[chroIndex][position] = newValue;
	
	LOG("Exit");
	return 0;
}

//int ga_mutation(struct GA_POOL* gaPoolPtr, int chroIndex, int position)
//{
//	GA_TYPE tmp;
//
//	// Checking
//	if(chroIndex >= gaPoolPtr->poolSize || position >= gaPoolPtr->chroLen)
//		return -1;
//
//	tmp = gaPoolPtr->pool[chroIndex][position];
//	tmp = 1 - (tmp - '0') + '0';
//	gaPoolPtr->pool[chroIndex][position] = tmp;
//
//	return 0;
//}

int ga_reproduction(struct GA_POOL* gaPoolPtr, int chroIndex)
{
	int retValue = 0;
	GA_TYPE* tmpChro = NULL;
	void* allocTmp = NULL;
	
	LOG("Enter");
	LOG("chroIndex: %d", chroIndex);

	// Checking
	if(chroIndex >= gaPoolPtr->poolSize)
	{
		retValue = -1;
		goto RET;
	}
	
	// Memory allocation
	tmpChro = (GA_TYPE*)malloc(sizeof(GA_TYPE) * gaPoolPtr->chroLen);
	if(tmpChro == NULL)
	{
		retValue = -1;
		goto RET;
	}

	allocTmp = realloc(gaPoolPtr->pool, sizeof(GA_TYPE*) * (gaPoolPtr->poolSize + 1));
	if(allocTmp == NULL)
	{
		retValue = -1;
		goto RET;
	}
	else
	{
		gaPoolPtr->poolSize += 1;
		gaPoolPtr->pool = (GA_TYPE**)allocTmp;
		allocTmp = NULL;
	}

	// Copy and assign
	ga_copy(tmpChro, gaPoolPtr->pool[chroIndex], 0, gaPoolPtr->chroLen);
	gaPoolPtr->pool[gaPoolPtr->poolSize - 1] = tmpChro;
	tmpChro = NULL;
	
	// Assign chromosome index
	retValue = gaPoolPtr->poolSize - 1;

RET:
	if(tmpChro != NULL)
		free(tmpChro);

	if(allocTmp != NULL)
		free(allocTmp);
	
	LOG("Exit");
	return retValue;
}

int ga_crossover(struct GA_POOL* gaPoolPtr, int chroIndex1, int chroIndex2, int cut)
{
	int i, j, k;
	int retValue = 0;

	GA_TYPE* parent[2];
	GA_TYPE* cross[4];

	void* allocTmp = NULL;
	
	LOG("Enter");
	LOG("chroIndex1: %d, chroIndex2: %d, cut: %d", chroIndex1, chroIndex2, cut);
	LOG("Pool size: %d", gaPoolPtr->poolSize);
	LOG("ChroLen: %d", gaPoolPtr->chroLen);

	// Checking
	if(chroIndex1 >= gaPoolPtr->poolSize || chroIndex2 >= gaPoolPtr->poolSize || cut >= gaPoolPtr->chroLen)
	{
		LOG("Checking failed");
		retValue = -1;
		goto RET;
	}

	// Zero memory
	LOG("Zero memory");
	memset((void*)cross, 0, sizeof(GA_TYPE*) * 4);

	// Set parent
	LOG("Set parent");
	parent[0] = gaPoolPtr->pool[chroIndex1];
	parent[1] = gaPoolPtr->pool[chroIndex2];
	
	// Memory allocation
	LOG("Memory allocation");
	for(i = 0; i < 4; i++)
	{
		cross[i] = (GA_TYPE*)malloc(sizeof(GA_TYPE) * gaPoolPtr->chroLen);
		if(cross[i] == NULL)
		{
			retValue = -1;
			goto RET;
		}
	}
	
	LOG("Resize ga pool");
	allocTmp = realloc(gaPoolPtr->pool, sizeof(GA_TYPE*) * (gaPoolPtr->poolSize + 4));
	if(allocTmp == NULL)
	{
		retValue = -1;
		goto RET;
	}
	else
	{
		gaPoolPtr->poolSize += 4;
		gaPoolPtr->pool = (GA_TYPE**)allocTmp;
		allocTmp = NULL;
	}

	// Crossover
	LOG("Crossover");
	k = 0;
	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 2; j++)
		{
			ga_copy(cross[k], parent[i], 0, cut + 1);
			ga_copy(cross[k], parent[j], cut + 1, gaPoolPtr->chroLen - cut - 1);
			k++;
		}
	}

	// Assign child
	LOG("Assign child");
	for(i = 0; i < 4; i++)
	{
		gaPoolPtr->pool[gaPoolPtr->poolSize - 1 - i] = cross[i];
		cross[i] = NULL;
	}
	
	// Assign first child chromosome index
	retValue = gaPoolPtr->poolSize - 4;
	LOG("Assign first child chromosome index: %d", retValue);

RET:
	for(i = 0; i < 4; i++)
	{
		if(cross[i] != NULL)
			free(cross[i]);
	}

	if(allocTmp != NULL)
		free(allocTmp);
	
	LOG("Exit");
	return retValue;
}

void ga_copy(GA_TYPE* dst, GA_TYPE* src, int startIndex, int size)
{
	int i;
	
	LOG("Enter");
	for(i = 0; i < size; i++)
	{
		dst[i + startIndex] = src[i + startIndex];
	}
	LOG("Exit");
}

int ga_order(struct GA_POOL* gaPoolPtr, double (*fitness)(GA_TYPE* chro, int chroLen))
{
	int i, j;
	GA_TYPE* tmp;
	
	LOG("Enter");
	for(i = 0; i < gaPoolPtr->poolSize - 1; i++)
	{
		for(j = 0; j < (gaPoolPtr->poolSize - 1) - i; j++)
		{
			if(fitness(gaPoolPtr->pool[j], gaPoolPtr->chroLen) < fitness(gaPoolPtr->pool[j + 1], gaPoolPtr->chroLen))
			{
				tmp = gaPoolPtr->pool[j];
				gaPoolPtr->pool[j] = gaPoolPtr->pool[j + 1];
				gaPoolPtr->pool[j + 1] = tmp;
			}
		}
	}
	
	LOG("Exit");
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
		printf("%d ", gaPoolPtr->pool[chroIndex][i]);
	}

	return 0;
}


int ga_insert(struct GA_POOL* gaPoolPtr, GA_TYPE* chro, int chroLen)
{
	int i;
	int retValue = 0;
	GA_TYPE* tmpChro = NULL;
	void* allocTmp = NULL;
	
	LOG("Enter");

	// Checking chromosome length
	if(gaPoolPtr->chroLen > 0 && gaPoolPtr->chroLen != chroLen)
		return -1;

	// Memory allocation
	tmpChro = malloc(sizeof(GA_TYPE) * chroLen);
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
	allocTmp = realloc(gaPoolPtr->pool, sizeof(GA_TYPE*) * (gaPoolPtr->poolSize + 1));
	if(allocTmp == NULL)
	{
		retValue = -1;
		goto RET;
	}
	else
	{
		gaPoolPtr->poolSize++;
		gaPoolPtr->pool = (GA_TYPE**)allocTmp;
		allocTmp = NULL;

		gaPoolPtr->pool[gaPoolPtr->poolSize - 1] = tmpChro;
		tmpChro = NULL;
	}

	// Assign chromosome index
	retValue = gaPoolPtr->poolSize - 1;

RET:
	if(allocTmp != NULL)
		free(allocTmp);

	if(tmpChro != NULL)
		free(tmpChro);
	
	LOG("Exit");
	return retValue;
}

int ga_create(struct GA_POOL* gaPoolPtr, int chroLen)
{
	LOG("Enter");

	gaPoolPtr->chroLen = chroLen;
	gaPoolPtr->poolSize = 0;
	gaPoolPtr->pool = NULL;
	
	LOG("Exit");
	return 0;
}

int ga_delete(struct GA_POOL* gaPoolPtr)
{
	int i;
	
	LOG("Enter");
	for(i = 0; i < gaPoolPtr->poolSize; i++)
	{
		if(gaPoolPtr->pool[i] != NULL)
			free(gaPoolPtr->pool[i]);
	}
	if(gaPoolPtr->pool != NULL)
		free(gaPoolPtr->pool);
	LOG("Exit");
	return 0;
}
