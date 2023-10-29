#pragma once
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
//#include "overlay.hpp"
#include <iostream>
#include <fstream>
#include <cstdint>
#include "communication_struct.h"
#define DEVICE_FILE "/dev/TaxiDriver"

extern int file_desc;
int open_device(void);
void close_device(void);
int get_pid(const char *program_name);

template <typename T>
T RPM(uintptr_t address)
{
    struct s_RPM args;
    args.addr = address;
    args.size = sizeof(T);
    args.out = 0;
    args.out_addr = &args.out;
    
    uint64_t ret = ioctl(file_desc, IOCTL_RPM, &args);
    if (ret < 0) {
        perror("Revird: RPM failed.");
        close(file_desc);
        return 0;
    }
    return (T)args.out;
}

template <typename T>
void WPM(uintptr_t address, T value)
{
    struct s_WPM args_wpm;
    args_wpm.addr = address;
    args_wpm.size = sizeof(T);
    args_wpm.value = value;
    uint64_t ret = ioctl(file_desc, IOCTL_WPM, &args_wpm);
    if (ret < 0) {
        perror("Revird: WPM failed.");
        close(file_desc);
        return;
    }
    return;
}

void WPM(uintptr_t addr, uintptr_t value, ssize_t size);
int open_process(int pid);
uintptr_t get_module(const char *mod);
#if KERNEL_UPSTREAM == 0
uintptr_t get_pid_module(int pid, const char *mod);
#if TESTING==1
uintptr_t VIRT_TO_PHYS(uintptr_t vaddr);
uintptr_t PHYS_TO_VIRT(uintptr_t paddr);
#endif
#endif