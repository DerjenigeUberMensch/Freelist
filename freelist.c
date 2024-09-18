
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freelist.h"


FreeList *
FreeListCreate(
        uint64_t bytes_length
        )
{   
    FreeList *list = malloc(sizeof(FreeList));
    if(list)
    {
        list->list = calloc(1, bytes_length);
        list->bytes = bytes_length;
        if(!list->list)
        {   
            free(list);
            list = NULL;
        }
    }
    return list;
}

int
FreeListCreateFill(
        FreeList *list,
        uint64_t bytes_length
        )
{   
    memset(list, 0, bytes_length);
    list->list = calloc(1, bytes_length);
    list->bytes = bytes_length;

    if(list->list)
    {   return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void
FreeListDestroy(
        FreeList *list_fill
        )
{
    free(list_fill);
    FreeListDestroyFilled(list_fill);
}

void
FreeListDestroyFilled(
        FreeList *list_fill
        )
{
    free(list_fill->list);
}


int
FreeListResize(
        FreeList *list,
        uint64_t bytes_length
        )
{
    if(list->bytes == bytes_length)
    {   return EXIT_SUCCESS;
    }
    void *x = realloc(list->list, bytes_length);
    if(x)
    {
        if(list->bytes < bytes_length)
        {   memset((uint8_t *)x + list->bytes, 0, bytes_length - list->bytes);
        }
        list->list = x;
        list->bytes = bytes_length;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int
FreeListResizeSmart(
        FreeList *list,
        uint64_t bytes_length
        )
{
    if(list->bytes == bytes_length)
    {   return EXIT_SUCCESS;
    }

    uint64_t bytes;

    /* Log n, smoothing */
    if(list->bytes > bytes_length)
    {   bytes = list->bytes - (list->bytes - bytes_length) / 2;
    }
    else
    {   bytes = bytes_length + (bytes_length - list->bytes) / 2;
    }
    return FreeListResize(list, bytes);
}

uint64_t
FreeListNextFree(
        FreeList *list,
        uint64_t index_start,
        uint8_t *found
        )
{
    const unsigned int CHAR_BIT_COUNT = 8;
    uint64_t i = 0;

    for(i = index_start; i < list->bytes * CHAR_BIT_COUNT; ++i)
    {
        if(FreeListIsFree(list, i))
        {   
            if(*found)
            {   *found = 1;
            }
            return i;
        }
    }
    if(*found)
    {   *found = 0;
    }
    return 0;
}

uint64_t
FreeListPrevFree(
        FreeList *list,
        uint64_t index_end,
        uint8_t *found
        )
{
    const unsigned int CHAR_BIT_COUNT = 8;
    uint64_t i = 0;

    for(i = list->bytes * CHAR_BIT_COUNT; i > index_end; --i)
    {
        if(FreeListIsFree(list, i))
        {
            if(*found)
            {   *found = 1;
            }
            return i;
        }
    }
    if(*found)
    {   *found = 0;
    }
    return 0;
}

void
FreeListSetFree(
        FreeList *list,
        uint64_t index
        )
{
    const unsigned int CHAR_BIT_COUNT = 8;
    uint8_t *i = list->list + (index / CHAR_BIT_COUNT);
    *i &= ~(uint8_t)(1 << (index % CHAR_BIT_COUNT));
}

void
FreeListSetUsed(
        FreeList *list,
        uint64_t index
        )
{
    const unsigned int CHAR_BIT_COUNT = 8;
    uint8_t *i = list->list + (index / CHAR_BIT_COUNT);
    *i |= (1 << (index % CHAR_BIT_COUNT));
}

int
FreeListIsFree(
        FreeList *list,
        uint64_t index
        )
{
    const unsigned int CHAR_BIT_COUNT = 8;
    const uint8_t *i = list->list + (index / CHAR_BIT_COUNT);
    return *i & (1 << (index % CHAR_BIT_COUNT));
}
