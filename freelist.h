#ifndef __FREE__LIST__H_
#define __FREE__LIST__H_


#include <stdbool.h>
#include <stdint.h>


typedef struct FreeList FreeList;

struct
FreeList
{
    uint8_t *list;
    uint64_t bytes;
};

/*
 * 
 * NOTE: bytes_length does not have to align to any boundary.
 *
 * RETURN: FreeList * on Success.
 * RETURN: NULL on Failure.
 */
FreeList *
FreeListCreate(
        uint64_t bytes_length
        );
/*
 * NOTE: bytes_length does not have to align to any boundary.
 *
 * RETURN: EXIT_SUCCESS on Success.
 * RETURN: EXIT_FAILURE on Failure.
 */
int
FreeListCreateFill(
        FreeList *list_fill,
        uint64_t bytes_length
        );

void
FreeListDestroy(
        FreeList *list_fill
        );

void
FreeListDestroyFilled(
        FreeList *list_fill
        );


/*
 * RETURN: EXIT_SUCCESS on Success.
 * RETURN: EXIT_FAILURE on Failure.
 */
int
FreeListResize(
        FreeList *list,
        uint64_t bytes_length
        );
/*
 * RETURN: EXIT_SUCCESS on Success.
 * RETURN: EXIT_FAILURE on Failure.
 */
int
FreeListResizeSmart(
        FreeList *list,
        uint64_t bytes_length
        );

/* Gets the Next Free location of the byte list.
 *
 * Ex:
 * 
 * FreeList bit(s)
 * 1111 0111 1111 1111
 *      ^
 * uint64_t num = FreeListNextFree(MyFreeList);
 *
 * // Num returned in this case is 5.
 * assert(num == 5)
 *
 *
 * RETURN: Free Number/index.
 */
uint64_t
FreeListNextFree(
        FreeList *list,
        uint64_t index_start,
        uint8_t *found
        );

/* Reversely calculates the Previous Free location of the byte list.
 *
 * Ex:
 * 
 * FreeList bit(s)
 * 1111 0111 1111 1111
 *      ^
 * uint64_t num = FreeListPrevFree(MyFreeList);
 *
 * // Num returned in this case is 5.
 * assert(num == 5)
 *
 *
 * RETURN: Free Number/index.
 */
uint64_t
FreeListPrevFree(
        FreeList *list,
        uint64_t index_end,
        uint8_t *found
        );

/*
 * Sets a index as 'free'.
 */
void
FreeListSetFree(
        FreeList *list,
        uint64_t index
        );

/*
 * Sets a index as 'not free'.
 */
void
FreeListSetUsed(
        FreeList *list,
        uint64_t index
        );

/*
 * Checks if a index is free.
 */
int
FreeListIsFree(
        FreeList *list,
        uint64_t index
        );



#endif
