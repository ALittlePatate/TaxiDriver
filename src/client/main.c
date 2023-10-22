#include "memory.h"
#include <stdint.h>

int main() {
    if (!open_device())
	return -1;

    int pid = 55803;
    if (!open_process(pid))
	return -1;
    
    uintptr_t addr = get_module("nsnake");
    printf("module : 0x%lx\n", addr);
    struct s_WPM args_wpm;
    args_wpm.addr = 0x55a813479d70;
    args_wpm.size = 4;
    args_wpm.value = (uintptr_t)667;
    WPM(args_wpm);
    
    int out = (int)RPM(0x55a813479d70, sizeof(int));
    printf("Value from RPM: %d\n", out);
    
    close_device();
    return 0;
}
