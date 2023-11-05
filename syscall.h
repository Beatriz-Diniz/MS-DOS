#ifndef SYSCALL_H
#define SYSCALL_H
#define SYS_WRITE 0

/* void  syscall_handler(void); */
void __attribute__((naked)) sys_write(void);
void __attribute__((naked)) syscall_handler(int, int, int, int);

/* SYSCALL_H */
#endif
