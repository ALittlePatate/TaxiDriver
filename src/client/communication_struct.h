#pragma once

#ifndef DRIVER
#include <stdint.h>
#include <stdio.h>
#endif

#define IOCTL_OPENPROC  _IOW('k', 1, int)
#define IOCTL_GETMODULE _IOW('k', 2, const char*)
#define IOCTL_RPM       _IOW('k', 3, t_RPM)
#define IOCTL_WPM       _IOW('k', 4, t_WPM)

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
