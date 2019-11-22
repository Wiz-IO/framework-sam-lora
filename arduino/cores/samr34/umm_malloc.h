/* ----------------------------------------------------------------------------
 * 
 *  umm_malloc.h - a memory allocator for embedded systems (microcontrollers)
 *  https://github.com/rhempel/umm_malloc
 *
 *  The MIT License (MIT)
 * 
 *  edit: Georgi Angelov
 * 
 * ----------------------------------------------------------------------------
 */

#ifndef UMM_MALLOC_H
#define UMM_MALLOC_H

#ifdef __cplusplus
extern "C"
{
#endif

//#define DISABLE_UMM

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <samr3.h>

/* ------------------------------------------------------------------------ */

void  umm_init( void );
void *umm_malloc( size_t size );
void *umm_calloc( size_t num, size_t size );
void *umm_realloc( void *ptr, size_t size );
void  umm_free( void *ptr );

/* ------------------------------------------------------------------------ */

#define UMM_H_ATTPACKPRE
#define UMM_H_ATTPACKSUF        __attribute__((__packed__))

#define UMM_BEST_FIT
#undef  UMM_FIRST_FIT

#define UMM_CRITICAL_ENTRY()    __disable_irq()
#define UMM_CRITICAL_EXIT()     __enable_irq()

#define INTEGRITY_CHECK() 0

#define DBGLOG_DEBUG(format, ...)
#define DBGLOG_TRACE(format, ...)

extern uint32_t _heap_begin;
extern uint32_t _heap_size;

#ifdef __cplusplus
}
#endif

#endif /* UMM_MALLOC_H */
