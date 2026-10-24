/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <print>

#include "mm.h"
#include "memlib.h"

using namespace std;

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ov4.org",
    /* First member's full name */
    "_ov4",
    /* First member's email address */
    "ov4@ov4.org",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

constexpr bool OV4_VERBOSE = false;

/* rounds up to the nearest multiple of ALIGNMENT */
inline size_t align(size_t size)
{
    return (((size) + (ALIGNMENT-1)) & ~0x7);
}

constexpr size_t SIZE_T_SIZE = 8;

inline bool is_alloc(size_t status)
{
    return (status&0x1);
}
inline bool is_prev_alloc(size_t status)
{
    return (status&0x2) >> 1;
}
inline void set_status(size_t &status, bool is_alloc)
{
    if (is_alloc)
        status |= 0x1;
    else
        status &= 0xfffffffe;
}
inline void set_prev_status(size_t &status, bool is_prev_alloc)
{
    if (is_prev_alloc)
        status |= 0x2;
    else
        status &= 0xfffffffd;
}
inline size_t get_size(size_t status)
{
    // ignore lowest 2 bits
    return status&0xfffffffc;
}
inline void set_size(size_t &status, size_t size)
{
    status &= 0x3;
    status |= (size & 0xfffffffc);
}

template< class... Args >
void loggerln( std::format_string<Args...> fmt, Args&&... args ) { if constexpr (OV4_VERBOSE) println(fmt, args...); }

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    return 0;
}

void mm_quit(int code, int line)
{
    loggerln("exit, line: {}", line);
    exit(code);
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    loggerln("----------");
    size_t newsize = align(size + SIZE_T_SIZE);
    loggerln("newsize: {}", newsize);
    //void *p = mem_sbrk(newsize);
    char *p = (char*)mem_heap_lo();
    while (true)
    {
        size_t status = 0;
        loggerln("offset: 0x{:x}", (size_t)p-(size_t)mem_heap_lo());

        // out of heap
        if (p >= mem_heap_hi())
        {
            loggerln("sbrk");
            mem_sbrk(newsize);
            set_size(status, newsize);
            set_status(status, true);
            *(size_t*)p = status;
            void *ret = (void*)((char*)p + SIZE_T_SIZE);
            loggerln("return: 0x{:x}" , (size_t)ret-(size_t)mem_heap_lo());
            return ret;
        }
        status = *(size_t*)p;
        if (is_alloc(status))
        {
            p += get_size(status);
            continue;
        }

        mm_quit(-1, __LINE__);
        
    }


    if (p == (void *)-1)
	    return NULL;
    else {
        
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}














