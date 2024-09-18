# Bitlist

Essentially a more convulete method for bitflags;

## Example
```C

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "freelist.h"

int
main(void)
{
    const int LIST_LENGTH = 800;
    const int LIST_BYTE_LENGTH = 100;
    FreeList list;
    int status;

    status = FreeListCreateFill(&list, LIST_BYTE_LENGTH);

    if(status != EXIT_SUCCESS)
    {
        printf("Failed to allocate list\n");
        return EXIT_FAILURE;
    }

    int i;
    for(i = 0; i < LIST_LENGTH; ++i)
    {   FreeListSetUsed(&list, i); 
    }

    const unsigned long int RANDOM_INDEX = 5;
    FreeListSetFree(&list, RANDOM_INDEX);

    const int START_SEARCH_INDEX = 0;
    uint8_t found = 0;
    uint64_t index = FreeListNextFree(&list, START_SEARCH_INDEX, &found);

    printf("Found?:         %s\n", found ? "true" : "false");
    printf("Index:          %lu\n", index);
    printf("Correct Index:  %lu\n", RANDOM_INDEX);

    FreeListDestroyFilled(&list);
    return EXIT_SUCCESS;
}
