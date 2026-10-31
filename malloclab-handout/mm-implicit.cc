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
#include <unistd.h>

#include "util/mm-implicit.h"

#include "mm.h"
#include "memlib.h"

using namespace std;

void print_list();

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
    loggerln("---------- malloc ----------");
    loggerln("argument: {}", size);
    print_list();
    size_t newsize = align(size + SIZE_T_SIZE);
    loggerln("newsize: {}", newsize);
    size_t p = (size_t)mem_heap_lo();
    bool prev_alloc = true;

    while (true)
    {
        //if (newsize == 4080) sleep(1);
        size_t status = 0;
        loggerln("offset: {}", (size_t)p-(size_t)mem_heap_lo());

        // out of heap
        if (p >= (size_t)mem_heap_hi())
        {
            // then increase heap
            // and set current pos to be free
            // so it can be alloc'd soon
            loggerln("before: max addr {}", offset((size_t)mem_heap_hi()));
            mem_sbrk(newsize);
            loggerln("sbrk {}", newsize);
            loggerln("max addr {}", offset((size_t)mem_heap_hi()));

            set_size(status, newsize);
            set_alloc(status, false);
            set_prev_alloc(status, true); // TODO
            *(size_t*)p = status;
        }
        status = *(size_t*)p;
        if (is_alloc(status))
        {
            p += get_size(status);
            prev_alloc = true;
            continue;
        }
        prev_alloc = false;

        if (get_size(status) < newsize)
        {
            next(p);
            continue;
        }

        size_t oldsize = get_size(status);

        set_size(status, newsize);
        set_alloc(status, true);
        set_prev_alloc(status, true); // TODO
        *(size_t*)p = status;
        void *ret = (void*)((char*)p + SIZE_T_SIZE);


        // if we don't use all of free space
        // split free block into 2 parts
        if (oldsize > newsize)
        {
            size_t free_status = 0;
            set_size(free_status, oldsize - newsize);
            set_alloc(free_status, false);
            set_prev_alloc(free_status, true);
            size_t free_header_p = p+newsize;
            *(size_t*)(free_header_p) = free_status;
            sync_footer(free_header_p);
            loggerln("split free: free begins {}", offset(free_header_p));
        }

        loggerln("return: {}" , (size_t)ret-(size_t)mem_heap_lo());
        return ret;
        
    }

}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if (ptr == nullptr) return;

    loggerln("---------- free ----------");

    loggerln("argument: {}", (size_t)ptr-(size_t)mem_heap_lo());

    print_list();

    size_t header = (size_t)ptr-SIZE_T_SIZE;
    size_t next_header = header + get_size(*(size_t*)header);

    if (next_header > (size_t)mem_heap_hi()) next_header = 0;

    loggerln("header: {}", offset(header));
    loggerln("header size: {}", get_size(*(size_t*)header));
    loggerln("next: {}", next_header != 0 ? offset(next_header) : 0);

    // free current block
    set_alloc(*(size_t*)header, false);
    sync_footer(header);

    // if next_header is free
    if ((next_header != 0) 
        && (!is_alloc(*(size_t*)next_header)))
    {
        size_t newsize = get_size(*(size_t*)header) + get_size(*(size_t*)next_header);
        loggerln("next free: newsize {}", newsize);
        set_size(*(size_t*)header, newsize);
        sync_footer(header);
    }

    // if prev exists
    // and is free
    if (!is_prev_alloc(*(size_t*)header)
        && header != (size_t)mem_heap_lo())
    {

        loggerln("handling prev free");

        size_t prev_footer = header - SIZE_T_SIZE;
        size_t prev_header = header - get_size(*(size_t*)prev_footer);

        loggerln("prev_footer: {}", offset(prev_footer));
        loggerln("prev footer size: {}", get_size(*(size_t*)prev_footer));

        loggerln("ok 0");
        size_t newsize = get_size(*(size_t*)prev_header) + get_size(*(size_t*)header);
        loggerln("ok1");
        set_size(*(size_t*)prev_header, newsize);
        loggerln("ok2");
        sync_footer(prev_header);
    }
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












