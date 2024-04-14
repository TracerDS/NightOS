#pragma once

#ifndef __NOS_ASSEMBLY_H__
#define __NOS_ASSEMBLY_H__

#ifdef __cplusplus
extern "C" {
#endif

static inline void kasmAdd(auto from, auto to) {
    __asm__ __volatile__ (
        "add %1, %0"
        : "+r" (to)
        : "r" (from)
    );
}

static inline void kasmMov(auto from, auto to) {
    __asm__ __volatile__ (
        "mov %1, %0"
        : "+r" (to)
        : "r" (from)
    );
}
static inline void kasmSub(auto from, auto to) {
    __asm__ __volatile__ (
        "sub %1, %0"
        : "+r" (to)
        : "r" (from)
    );
}

#ifdef __cplusplus
}
#endif

#endif