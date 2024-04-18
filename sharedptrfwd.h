//
// Created by Surya on 13-04-2024
//
/**
 * @file sharedptrfwd.inl
 * @brief Header file for shared pointer functionality with Grbage Collection
 * @date 13-04-2024
 */
#ifndef SHARED_PTR_F
#define SHARED_PTR_F

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
#include <pthread.h>

#ifndef _ALLOCATION_SHARED
#define _ALLOCATION_SHARED
#define _SUPPRESS_UNUSED    __attribute__((unused))

#ifndef PROGRAM_LOG_GEN
#define PROGRAM_LOG_GEN

#endif

typedef struct _shared_allocator_build {
    void* acnt;
    size_t ref_cnt : 30;
    int static_cnt : 10;
    struct _shared_allocator_build** _collector[32];
    struct _shared_allocator_build* _ust;
}synchronized_ptr;

#endif

#ifndef _SHARED_DECL
#define _SHARED_DECL __cdecl
#endif
#define __forever__ 1
typedef synchronized_ptr* shared_ptr;


_SHARED_DECL __attribute__ ((__pure__))
synchronized_ptr* sync_with_dynamic_allocator(int _Size);

#ifndef SHARED_ATTRIBUTE_SYNCHRONIZED_CONTROLLER
#define SYNCHRONIZED_CONTROLLER 
#endif

_SHARED_DECL
void allocator_load(synchronized_ptr** _adrs);
_SHARED_DECL 
void move_ref(synchronized_ptr** _Src, synchronized_ptr** _Dest);

struct _mem_tf_info {
    int _mem_arr[32];
    int cnt;
};

_SUPPRESS_UNUSED
struct _mem_tf_info get_memory_ref_count();
_SHARED_DECL
void sharedAdjList();


#endif // SHARED_PTR_F