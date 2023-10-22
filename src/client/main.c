#include "memory.h"
#include <stdint.h>

int main() {
    if (!open_device())
	return -1;

    int pid = get_pid("nsnake");
    if (!open_process(pid))
	return -1;
    
    uintptr_t addr = get_module("nsnake");
    printf("module : 0x%lx\n", addr);
    WPM(0x55a813479d70, 1337, sizeof(int));
    
    int out = (int)RPM(0x55a813479d70, sizeof(int));
    printf("Value from RPM: %d\n", out);
    
    close_device();
    return 0;
}
