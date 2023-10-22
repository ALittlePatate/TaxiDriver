#pragma once

#include "communication_struct.h"

int open_device(void);
void close_device(void);
int get_pid(const char *program_name);
void *RPM(uintptr_t address, ssize_t size); 
void WPM(uintptr_t addr, uintptr_t value, ssize_t size);
int open_process(int pid);
uintptr_t get_module(const char *mod);
