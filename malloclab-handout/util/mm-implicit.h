#pragma once

#include <cstring>

#include "../mm.h"
#include "../memlib.h"


using namespace std;

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
inline size_t align(size_t size)
{
    return (((size) + (ALIGNMENT-1)) & ~0x7);
}

inline constexpr size_t SIZE_T_SIZE = 8;

inline bool is_alloc(size_t status)
{
    return (status&0x1);
}
inline bool is_prev_alloc(size_t status)
{
    return (status&0x2) >> 1;
}
inline void set_alloc(size_t &status, bool is_alloc)
{
    if (is_alloc)
        status |= 0x1;
    else
        status &= 0xfffffffe;
}
inline void set_prev_alloc(size_t &status, bool is_prev_alloc)
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
inline size_t offset(size_t p)
{
    return (size_t)((size_t)p-(size_t)mem_heap_lo());
}
inline void next(size_t &p)
{
    p += get_size(*(size_t*)p);
}

template< class... Args >
void loggerln( std::format_string<Args...> fmt, Args&&... args ) 
{ 
#ifdef OV4_VERBOSE
    println(fmt, forward<Args>(args)...); 
#endif
}

template< class... Args >
void logger( std::format_string<Args...> fmt, Args&&... args ) 
{ 
#ifdef OV4_VERBOSE
    print(fmt, forward<Args>(args)...); 
#endif
}


void print_list()
{
#ifdef OV4_VERBOSE
    logger("print_list: ");
    size_t p = (size_t)mem_heap_lo();
    while (true)
    {
        if (p >= (size_t)mem_heap_hi())
            break;
        
        logger("{}({},{}) ", offset(p), is_alloc(*(size_t*)p) ? "alloc" : "free", get_size(*(size_t*)p));
        p += get_size(*(size_t*)p);
    }
    loggerln("");
#endif  
}

void sync_footer(size_t header)
{
    size_t footer = header + get_size(*(size_t*)header) - SIZE_T_SIZE;
    *(size_t*)footer = *(size_t*)header;
}