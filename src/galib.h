#ifndef _GALIB_H_
#define _GALIB_H_

typedef int GA_TYPE;

struct GA_POOL
{
	int chroLen;

	int poolSize;
	GA_TYPE** pool;
};

#ifdef __cplusplus
extern "C" {
#endif

int ga_create(struct GA_POOL* gaPoolPtr, int chroLen);
int ga_delete(struct GA_POOL* gaPoolPtr);
int ga_insert(struct GA_POOL* gaPoolPtr, GA_TYPE* chro, int chroLen);
int ga_order(struct GA_POOL* gaPoolPtr, double (*fitness)(GA_TYPE* chro, int chroLen));
int ga_kill_after(struct GA_POOL* gaPoolPtr, int killIndex);

int ga_crossover(struct GA_POOL* gaPoolPtr, int chroIndex1, int chroIndex2, int cut);
int ga_reproduction(struct GA_POOL* gaPoolPtr, int chroIndex);
int ga_edit_chro(struct GA_POOL* gaPoolPtr, int chroIndex, int position, GA_TYPE newValue);
//int ga_mutation(struct GA_POOL* gaPoolPtr, int chroIndex, int position);

int ga_print_chro(struct GA_POOL* gaPoolPtr, int chroIndex);

#ifdef __cplusplus
}
#endif

#endif
