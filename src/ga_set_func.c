#include <stdlib.h>

#include "galib.h"
#include "debug.h"

struct GA_SET ga_set_init(int startIndex, int endIndex)
{
	struct GA_SET tmpSet;

	tmpSet.startIndex = startIndex;
	tmpSet.endIndex = endIndex;

	return tmpSet;
}

int ga_set_crossover(struct GA_POOL gaPoolPtr, struct GA_SET gaSet, int chroIndex1, int chroIndex2, int cut)
{
	int i, j, k;
	int retValue = 0;
	int setLen;

	

	return retValue;
}

/*
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
*/
