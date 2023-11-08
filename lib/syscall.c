#include "syscall.h"
#include "lib.h"

// generates functions from syscall0 to syscall_6

// clang-format off

// argument registers in order
#define RN %rax
#define R1 %rdi
#define R2 %rsi
#define R3 %rdx
#define R4 %r10
#define R5 %r8
#define R6 %r9


#define FARGSN(n) long arg##n
#define FARGS1 FARGSN(1)
#define FARGS2 FARGS1, FARGSN(2)
#define FARGS3 FARGS2, FARGSN(3)
#define FARGS4 FARGS3, FARGSN(4)
#define FARGS5 FARGS4, FARGSN(5)
#define FARGS6 FARGS5, FARGSN(6)


#define FSTARTN(n) long syscall##n(syscall_num num
#define FSTART0(inner) FSTARTN(0) ) {inner}
#define FSTART1(inner) FSTARTN(1), FARGS1) {inner}
#define FSTART2(inner) FSTARTN(2), FARGS2) {inner}
#define FSTART3(inner) FSTARTN(3), FARGS3) {inner}
#define FSTART4(inner) FSTARTN(4), FARGS4) {inner}
#define FSTART5(inner) FSTARTN(5), FARGS5) {inner}
#define FSTART6(inner) FSTARTN(6), FARGS6) {inner}


#define MOVN(n) STR(movq %[arg##n])","STR(%R##n)";\n"
#define MOV0 "movq %[num], "STR(%RN)";\n"
#define MOV1 MOV0 MOVN(1)
#define MOV2 MOV1 MOVN(2)
#define MOV3 MOV2 MOVN(3)
#define MOV4 MOV3 MOVN(4)
#define MOV5 MOV4 MOVN(5)
#define MOV6 MOV5 MOVN(6)

#define ARGN(num) [arg##num] "g"((long)arg##num)
#define ARGLIST0 [num] "g"((long)num)
#define ARGLIST1 ARGLIST0, ARGN(1)
#define ARGLIST2 ARGLIST1, ARGN(2)
#define ARGLIST3 ARGLIST2, ARGN(3)
#define ARGLIST4 ARGLIST3, ARGN(4)
#define ARGLIST5 ARGLIST4, ARGN(5)
#define ARGLIST6 ARGLIST5, ARGN(6)


#define CLOBN(n) STR(R##n)
#define CLOB0 STR(RN)
#define CLOB1 CLOB0, CLOBN(1)
#define CLOB2 CLOB1, CLOBN(2)
#define CLOB3 CLOB2, CLOBN(3)
#define CLOB4 CLOB3, CLOBN(4)
#define CLOB5 CLOB4, CLOBN(5)
#define CLOB6 CLOB5, CLOBN(6)


#define FULL_ASM(n) \
MOV##n \
"syscall;\n"\
"movq %%rax, %[ret];\n"\
: [ret]"=g"(ret)\
: ARGLIST##n\
: CLOB##n


#define COMPLETE(n) FSTART##n(long ret; asm(FULL_ASM(n)); return ret;)


COMPLETE(0)
COMPLETE(1)
COMPLETE(2)
COMPLETE(3)
COMPLETE(4)
COMPLETE(5)
COMPLETE(6)

// clang-format on
