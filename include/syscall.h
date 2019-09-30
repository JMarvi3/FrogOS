#ifndef _SYSCALL_H
#define _SYSCALL_H

void* syscall(
    void* syscall_number,
    void* param1,
    void* param2,
    void* param3,
    void* param4,
    void* param5
);

#endif
