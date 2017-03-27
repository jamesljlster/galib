#ifndef _GALIB_H_
#define _GALIB_H_

typedef double GA_TYPE;

struct GA_POOL
{
	int chroLen;

	int poolSize;
	GA_TYPE** pool;
};

struct GA_SET
{
	int startIndex;
	int setLen;
};

#ifdef __cplusplus
extern "C" {
#endif

struct GA_SET ga_set_init(int startIndex, int setLen);

int ga_set_crossover(struct GA_POOL* gaPoolPtr, struct GA_SET gaSet, int chroIndex1, int chroIndex2, int cut);

int ga_create(struct GA_POOL* gaPoolPtr, int chroLen);
/**
 *	@fn		int ga_create(struct GA_POOL* gaPoolPtr, int chroLen);
 *	@brief	Create a GA pool.
 *	@param	gaPoolPtr:	GA data structure pointer.
 *	@param	chroLen:	Target chromosome length in the pool.
 *	@return	If error, the function would return -1, else return 0;
 */

int ga_delete(struct GA_POOL* gaPoolPtr);
/**
 *	@fn		int ga_delete(struct GA_POOL* gaPoolPtr);
 *	@brief	Delete GA pool.
 *	@param	gaPoolPtr:	GA data structure pointer.
 */

int ga_insert(struct GA_POOL* gaPoolPtr, GA_TYPE* chro, int chroLen);
/**
 *	@fn		int ga_insert(struct GA_POOL* gaPoolPtr, GA_TYPE* chro, int chroLen);
 *	@brief	Insert chromosome to GA pool.
 *	@param	gaPoolPtr:	GA data structure pointer.
 *	@param	chro:		Target chromosome.
 *	@param	chroLen:	Target chromosome length.
 *	@return	If function execute succeed, function would return the target chromosome index in GA pool, else return -1.
 */

int ga_remove(struct GA_POOL* gaPoolPtr, int chroIndex);
/**
 *	@fn		int ga_remove(struct GA_POOL* gaPoolPtr, int chroIndex);
 *	@brief	Remove target chromosome from GA pool.
 *	@param	gaPoolPtr:	GA data structure pointer.
 *	@param	chroIndex:	Target index of chromosome which assigned to remove.
 *	@return	If error, the function would return -1, else return 0.
 */

int ga_order(struct GA_POOL* gaPoolPtr, double (*fitness)(GA_TYPE* chro, int chroLen, void* arg), int inverse, void* arg);
/**
 *	@fn		int ga_order(struct GA_POOL* gaPoolPtr, double (*fitness)(GA_TYPE* chro, int chroLen));
 *	@brief	Order chromosomes in GA pool with given fitness function.
 *	@param	gaPoolPtr:	GA data structure pointer.
 *	@param	fitness:	Fitness function.
 *	@param	inverse:	If given inverse > 0, order operation would sort fitness from low to high.
 *	@return	If error, the function would return -1, else return 0.
 */
 
int ga_kill_after(struct GA_POOL* gaPoolPtr, int killIndex);
/**
 *	@fn		int ga_kill_after(struct GA_POOL* gaPoolPtr, int killIndex);
 *	@brief	Kill chromosomes in GA pool after given index.
 *	@param	gaPoolPtr:	GA data structure pointer.
 *	@param	killIndex:	Kill all chromosome in pool after this index.
 *	@return	If error, the function would reutrn -1, else return 0.
 */

int ga_crossover(struct GA_POOL* gaPoolPtr, int chroIndex1, int chroIndex2, int cut);
/**
 *	@fn		int ga_crossover(struct GA_POOL* gaPoolPtr, int chroIndex1, int chroIndex2, int cut);
 *	@brief	Processing crossover in GA pool.
 *	@param	gaPoolPtr:	GA data structure pointer.
 *	@param	chroIndex1:	Parent chromosome index in the pool.
 *	@param	chroIndex2:	Parent chromosome index in the pool.
 *	@param	cut:		Cut point on chromosomes.
 *	@reutrn	If error, the function would return -1, else return the first child index in the pool.
 */
 
int ga_reproduction(struct GA_POOL* gaPoolPtr, int chroIndex);
/**
 *	@fn		int ga_reproduction(struct GA_POOL* gaPoolPtr, int chroIndex);
 *	@brief	Processing reproduction in GA pool.
 *	@param	gaPoolPtr:	GA data structure pointer.
 *	@param	chroIndex:	Target chromosome index in the pool.
 *	@return	If error, the function would return -1, else return the new reproduced chromosome index.
 */

int ga_edit_chro(struct GA_POOL* gaPoolPtr, int chroIndex, int position, GA_TYPE newValue);
/**
 *	@fn		int ga_edit_chro(struct GA_POOL* gaPoolPtr, int chroIndex, int position, GA_TYPE newValue);
 *	@brief	Edit the target chromosome in the pool.
 *	@param	gaPoolPtr:	GA data structure pointer.
 *	@param	chroIndex:	Target chromosome index in the pool that would be edit.
 *	@param	position:	The position index of target chromosome that would be assigned with new value.
 *	@return	If error, the function would return -1, else return 0.
 */

#ifdef __cplusplus
}
#endif

#endif
