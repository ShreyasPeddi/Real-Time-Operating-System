#include "k_mem.h"
#include "k_task.h"
#include "common.h"
#include <stdio.h>

extern uint32_t _estack;
extern uint32_t _Min_Stack_Size;
extern uint32_t _img_end;

// from kernel.c
extern int kernel_init;
extern task_t new_TID;
extern task_t current_TID;

int mem_initialized = 0;

// tested - sizeof(MEM_BLOCK) = 24 Bytes
const uint32_t MEM_BLOCK_SIZE = 24;

typedef struct MEM_BLOCK {
    task_t owner;            	// TID of task that owns this memory block
    uint8_t allocated;       	// flag - allocated => 1, free => 0

    struct MEM_BLOCK* next;  	// ptr to the next MEM_BLOCK
    struct MEM_BLOCK* prev;  	// ptr to the prev MEM_BLOCK


    uint32_t* start_location;	// ptr to memory start location on heap
    uint32_t size;           	// size of USABLE!!! memory in this block of the usable memory (NOT INCLUDING sizeof(MEM_BLOCK)!!!)
}
MEM_BLOCK;

static MEM_BLOCK* free_list = NULL;  // ptr to first block in FREE linked list

static uint32_t* heap_start = NULL;
static uint32_t* heap_end = NULL;


static uint32_t align_to_word(uint32_t size);

int k_mem_init(void) {
//	int x = sizeof(MEM_BLOCK);
    if (mem_initialized || !kernel_init) {
        return RTX_ERR;
    }

    // heap bounds fr
    heap_start = (uint32_t*)&_img_end;
    heap_end = (uint32_t*)((uint32_t)&_estack - (uint32_t)&_Min_Stack_Size);

    // first block covers the ENTIRE HEAP
    free_list = (MEM_BLOCK*)heap_start;
    free_list->size = (uint32_t)heap_end - (uint32_t)heap_start - MEM_BLOCK_SIZE;
    free_list->owner = TID_NULL;
    free_list->allocated = 0;
    free_list->next = NULL;
    free_list->prev = NULL;
    free_list->start_location = free_list;

    mem_initialized = 1;
    return RTX_OK;
}


static MEM_BLOCK* last_alloc = NULL;
static uint32_t alloc_min_size = 0;
// Next fit???? ("citcular" first fit)
void* k_mem_alloc(unsigned int size) {
    if (!mem_initialized || size == 0) {
        return NULL;
    }

    uint32_t aligned_size = align_to_word(size);

//    MEM_BLOCK* start = last_alloc && (last_alloc->size <= aligned_size) ? last_alloc->next : free_list;
    MEM_BLOCK* start = last_alloc && (alloc_min_size <= aligned_size || last_alloc->size <= aligned_size) ? last_alloc->next : free_list;
//    MEM_BLOCK* start = last_alloc ? last_alloc->next : free_list;

    MEM_BLOCK* curr = start;
    MEM_BLOCK* prev = NULL;

    do {
        if (!curr->allocated && curr->size >= aligned_size) {
            if (curr->size >= aligned_size + MEM_BLOCK_SIZE + 4) {
                MEM_BLOCK* new_block = (MEM_BLOCK*)((uint32_t)curr + MEM_BLOCK_SIZE + aligned_size);
                new_block->size = curr->size - aligned_size - MEM_BLOCK_SIZE;
                new_block->owner = TID_NULL;
                new_block->allocated = 0;
                new_block->next = curr->next;
                new_block->prev = curr;
                new_block->start_location = new_block;

                if (curr->next) {
                    curr->next->prev = new_block;
                }

                curr->size = aligned_size;
                curr->next = new_block;
            }

            curr->allocated = 1;
            curr->owner = new_TID;
            last_alloc = curr;
            alloc_min_size = (last_alloc) ? ((alloc_min_size < aligned_size) ? alloc_min_size : aligned_size) : aligned_size;


            return (void*)((uint32_t)curr + MEM_BLOCK_SIZE);
        }

        prev = curr;
        curr = curr->next ? curr->next : free_list;
    } while (curr != start);

    return NULL;
}


int k_mem_dealloc(void* ptr) {
    if (!mem_initialized) {
        return RTX_ERR;
    }

    // no need to dealloc if it's already null
    if (ptr == NULL) {
        return RTX_OK;
    }

    MEM_BLOCK* block = (MEM_BLOCK*)((uint32_t)ptr - MEM_BLOCK_SIZE);
    if ((uint32_t)block < (uint32_t)heap_start || (uint32_t)block >= (uint32_t)heap_end) {
        return RTX_ERR;
    }

    if (block->start_location != block || (uint32_t*)((uint32_t)block + MEM_BLOCK_SIZE) != ptr) {
        return RTX_ERR;
    }

    if (!block->allocated) {
        return RTX_ERR;
    }

    if (block->owner != current_TID && current_TID != TID_NULL) {
        return RTX_ERR;
    }

    // mark as free
    block->allocated = 0;
    block->owner = TID_NULL;

    // set last_alloc to null so that first fit will now start from index 0 instead of last_alloc
    last_alloc = NULL;

    MEM_BLOCK* coalesce_start = block;
    // printf("Prev: %p\r\n", coalesce_start->prev);
    if (coalesce_start->prev != NULL && !coalesce_start->prev->allocated) {
    	coalesce_start = free_list;
    }
    coalesce(coalesce_start);

    return RTX_OK;
}



// coalesce adjacent free blocks
void coalesce(MEM_BLOCK* curr) {
//    MEM_BLOCK* curr = free_list;

    while (curr != NULL && curr->next != NULL) {
        if (!curr->allocated && !curr->next->allocated) {
        	// next block into curr block
            MEM_BLOCK* to_merge = curr->next;
            curr->size += MEM_BLOCK_SIZE + to_merge->size;
            curr->next = to_merge->next;
            if (to_merge->next != NULL) {
                to_merge->next->prev = curr;
            }
            // continue checking from here
        } else {
        	curr = curr->next;
        }
    }
}

// TODO - need to test
int k_mem_count_extfrag(unsigned int size) {
    if (!mem_initialized) {
        return 0;
    }

    uint32_t aligned_size = align_to_word(size);

    int count = 0;
    MEM_BLOCK* curr = free_list;

    while (curr != NULL) {
        if (!curr->allocated && curr->size < aligned_size) {
            count++;
        }
        curr = curr->next;
    }

    return count;
}




// align word size with boundary - 4Bytes
static uint32_t align_to_word(uint32_t size) {
    return (size + 3) & ~0x03; // bitwise NOT
}
