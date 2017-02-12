/*
 * word10.h
 *
 *  Created on: Sep 6, 2015
 *      Author: Tim Stark
 */

#once

typedef uint32_t   uint18_t;
typedef uint64_t   uint36_t;

typedef int32_t   sint18_t;
typedef int64_t   sint36_t;

typedef uint18_t   h10_t;
typedef uint36_t   w10_t;

#define W10_MASK    0777777777777LL
#define W10_LMASK   0777777000000LL
#define W10_RMASK   0000000777777LL
