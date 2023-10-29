#pragma once

#ifndef DRIVER
#include <stdint.h>
#include <stdio.h>
#endif

#define IOCTL_OPENPROC		_IOW('k', 1, int)
#define IOCTL_GETMODULE		_IOW('k', 2, const char*)
#define IOCTL_RPM			_IOW('k', 3, t_RPM)
#define IOCTL_WPM			_IOW('k', 4, t_WPM)

#if KERNEL_UPSTREAM == 0
#define IOCTL_GETPIDMODULE _IOWR('k', 5, t_PM)
#if TESTING == 1
#define IOCTL_VIRT_TO_PHYS _IOWR('k', 6, unsigned long)
#define IOCTL_PHYS_TO_VIRT _IOWR('k', 7, unsigned long)
#endif
#endif

typedef struct s_RPM
{
    uintptr_t addr;
    ssize_t size;
    uintptr_t out;
    uintptr_t *out_addr;
} t_RPM;

typedef struct s_WPM
{
    uintptr_t addr;
    ssize_t size;
    uintptr_t value;
} t_WPM;

#if KERNEL_UPSTREAM == 0
typedef struct s_PM {
    int pid;
    const char *mod;
} t_PM;
#endif
