#ifndef _GALIB_H_
#define _GALIB_H_

struct GA_POOL
{
	int chroLen;

	int poolSize;
	char** pool;
};

#ifdef __cplusplus
extern "C" {
#endif

int ga_create(struct GA_POOL* gaPoolPtr, int chroLen);
int ga_delete(struct GA_POOL* gaPoolPtr);
int ga_insert(struct GA_POOL* gaPoolPtr, char* chro, int chroLen);
int ga_order(struct GA_POOL* gaPoolPtr, double (*fitness)(char* chro, int chroLen));

int ga_crossover(struct GA_POOL* gaPoolPtr, int chroIndex1, int chroIndex2, int cut);
int ga_reproduction(struct GA_POOL* gaPoolPtr, int chroIndex);
int ga_mutation(struct GA_POOL* gaPoolPtr, int chroIndex);

int ga_print_chro(struct GA_POOL* gaPoolPtr, int chroIndex);

#ifdef __cplusplud
}
#endif

#endif
