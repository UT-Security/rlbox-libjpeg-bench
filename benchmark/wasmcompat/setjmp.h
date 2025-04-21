#ifndef	_SETJMP_EMULATE_H
#define	_SETJMP_EMULATE_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct jmp_buf {
    int dummy;
} jmp_buf;

#define setjmp(...) 0
#define longjmp(...) abort()

#ifdef __cplusplus
}
#endif

#endif