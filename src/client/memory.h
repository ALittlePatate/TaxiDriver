#pragma once

#include "communication_struct.h"

int open_device(void);
void close_device(void);
void *RPM(uintptr_t address, ssize_t size); 
void WPM(t_WPM args);
int open_process(int pid);
uintptr_t get_module(const char *mod);
