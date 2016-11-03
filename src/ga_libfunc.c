#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "galib.h"

void ga_copy(char* dst, char* src, int startIndex, int size);

int ga_kill_after(struct GA_POOL* gaPoolPtr, int killIndex)
{
	int retValue = 0;
	void* allocTmp = NULL;

	// Checking
	if(killIndex >= gaPoolPtr->poolSize)
	{
		retValue = -1;
		goto RET;
	}

	allocTmp = realloc(gaPoolPtr->pool, sizeof(char*) * (killIndex + 1));
	if(allocTmp == NULL)
	{
		retValue = -1;
		goto RET;
	}
	else
	{
		gaPoolPtr->poolSize = killIndex + 1;
		gaPoolPtr->pool = (char**)allocTmp;
		allocTmp = NULL;
	}

RET:
	if(allocTmp != NULL)
		free(allocTmp);

	return retValue;
}

int ga_mutation(struct GA_POOL* gaPoolPtr, int chroIndex, int position)
{
	char tmp;

	// Checking
	if(chroIndex >= gaPoolPtr->poolSize || position >= gaPoolPtr->chroLen)
		return -1;

	tmp = gaPoolPtr->pool[chroIndex][position];
	tmp = 1 - (tmp - '0') + '0';
	gaPoolPtr->pool[chroIndex][position] = tmp;

	return 0;
}

int ga_reproduction(struct GA_POOL* gaPoolPtr, int chroIndex)
{
	int retValue = 0;
	char* tmpChro = NULL;
	void* allocTmp = NULL;

	// Checking
	if(chroIndex >= gaPoolPtr->poolSize)
	{
		retValue = -1;
		goto RET;
	}
	
	// Memory allocation
	tmpChro = (char*)malloc(sizeof(char) * gaPoolPtr->chroLen);
	if(tmpChro == NULL)
	{
		retValue = -1;
		goto RET;
	}

	allocTmp = realloc(gaPoolPtr->pool, sizeof(char*) * (gaPoolPtr->poolSize + 1));
	if(allocTmp == NULL)
	{
		retValue = -1;
		goto RET;
	}
	else
	{
		gaPoolPtr->poolSize += 1;
		gaPoolPtr->pool = (char**)allocTmp;
		allocTmp = NULL;
	}

	// Copy and assign
	ga_copy(tmpChro, gaPoolPtr->pool[chroIndex], 0, gaPoolPtr->chroLen);
	gaPoolPtr->pool[gaPoolPtr->poolSize - 1] = tmpChro;
	tmpChro = NULL;

RET:
	if(tmpChro != NULL)
		free(tmpChro);

	if(allocTmp != NULL)
		free(allocTmp);

	return retValue;
}

int ga_crossover(struct GA_POOL* gaPoolPtr, int chroIndex1, int chroIndex2, int cut)
{
	int i, j, k;
	int retValue = 0;

	char* parent[2];
	char* cross[4];

	void* allocTmp = NULL;

	// Checking
	if(chroIndex1 >= gaPoolPtr->poolSize || chroIndex2 >= gaPoolPtr->poolSize || cut >= gaPoolPtr->chroLen)
	{
		retValue = -1;
		goto RET;
	}

	// Zero memory
	memset((void*)cross, 0, sizeof(char*) * 4);

	// Set parent
	parent[0] = gaPoolPtr->pool[chroIndex1];
	parent[1] = gaPoolPtr->pool[chroIndex2];
	
	// Memory allocation
	for(i = 0; i < 4; i++)
	{
		cross[i] = (char*)malloc(sizeof(char) * gaPoolPtr->chroLen);
		if(cross[i] == NULL)
		{
			retValue = -1;
			goto RET;
		}
	}

	allocTmp = realloc(gaPoolPtr->pool, sizeof(char*) * (gaPoolPtr->poolSize + 4));
	if(allocTmp == NULL)
	{
		retValue = -1;
		goto RET;
	}
	else
	{
		gaPoolPtr->poolSize += 4;
		gaPoolPtr->pool = (char**)allocTmp;
		allocTmp = NULL;
	}

	// Crossover
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
	for(i = 0; i < 4; i++)
	{
		gaPoolPtr->pool[gaPoolPtr->poolSize - 1 - i] = cross[i];
		cross[i] = NULL;
	}

RET:
	for(i = 0; i < 4; i++)
	{
		if(cross[i] != NULL)
			free(cross[i]);
	}

	if(allocTmp != NULL)
		free(allocTmp);

	return retValue;
}

void ga_copy(char* dst, char* src, int startIndex, int size)
{
	int i;

	for(i = 0; i < size; i++)
	{
		dst[i + startIndex] = src[i + startIndex];
	}
}

int ga_order(struct GA_POOL* gaPoolPtr, double (*fitness)(char* chro, int chroLen))
{
	int i, j;
	char* tmp;

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
