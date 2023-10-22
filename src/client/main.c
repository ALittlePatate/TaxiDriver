#include "memory.h"

int main() {
    if (!open_device())
	return -1;

    int pid = 55803;
    if (!open_process(pid))
	return -1;
    
    struct s_WPM args_wpm;
    args_wpm.addr = 0x55a813479d70;
    args_wpm.size = 4;
    args_wpm.value = (uintptr_t)667;
    WPM(args_wpm);
    
    struct s_RPM args;
    args.addr = 0x55a813479d70;
    args.size = sizeof(int);
    int val = (int)RPM(args);
    printf("Value from RPM: %d\n", val);
    
    close_device();
    return 0;
}
