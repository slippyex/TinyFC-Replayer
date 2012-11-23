#ifndef MYTYPES_H
#define MYTYPES_H


#include "Config.h"

///* A ``bool'' type for compilers that don't (yet) support one. */
//#ifndef FC_HAVE_BOOL
//  typedef int bool;
//
//  #if defined(true) || defined(false)
//    #error Better check include file ``mytypes.h''.
//    #undef true
//    #undef false
//  #endif
//  #define true 1
//  #define false 0
//#endif  /* FC_HAVE_BOOL */


/* Wanted: 8-bit signed/unsigned. */
typedef signed char sbyte;
typedef unsigned char ubyte;

typedef signed short int sword;
typedef unsigned short int uword;

// Wanted: 32-bit signed/unsigned.
typedef signed int sdword;
typedef unsigned int udword;

// Some common type shortcuts.
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long int ulong;


typedef void (*ptr2func)();


#endif
