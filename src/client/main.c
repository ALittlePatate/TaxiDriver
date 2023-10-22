#include "memory.h"

int main() {
    if (!open_device())
	return -1;

    int pid = 25801;
    if (!open_process(pid))
	return -1;
    
    struct s_RPM args;
    args.addr = 0x420;
    args.size = 4;
    int val = (int)RPM(args);
    printf("Value from RPM: %d\n", val);

    struct s_WPM args_wpm;
    args_wpm.addr = 0x420;
    args_wpm.size = 4;
    args_wpm.value = (uintptr_t)667;
    WPM(args_wpm);
    
    close_device();
    return 0;
}
