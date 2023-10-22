#pragma once

#include "communication_struct.h"

int open_device(void);
void close_device(void);
void *RPM(t_RPM args);
void WPM(t_WPM args);
int open_process(int pid);
