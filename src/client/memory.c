#include "memory.h"
#include <sys/ioctl.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_FILE "/dev/TaxiDriver"

static int file_desc;
int open_device(void)
{
    file_desc = open(DEVICE_FILE, O_RDWR);
    if (file_desc < 0) {
        perror("Revird: Failed to open the device.");
        return -1;
    }
    return 1;
}

void close_device(void)
{
    close(file_desc);
}

void *RPM(t_RPM args)
{
    int ret;
    
    ret = ioctl(file_desc, IOCTL_RPM, &args);
    if (ret < 0) {
        perror("Revird: RPM failed.");
        close(file_desc);
        return 0;
    }
    return (void *)ret;
}

void WPM(t_WPM args)
{
    int ret;
    
    ret = ioctl(file_desc, IOCTL_WPM, &args);
    if (ret < 0) {
        perror("Revird: WPM failed.");
        close(file_desc);
        return;
    }
    return;
}

int open_process(int pid)
{
    int ret;
    
    ret = ioctl(file_desc, IOCTL_OPENPROC, &pid);
    if (ret < 0) {
        perror("Revird: openprocess failed.");
        close(file_desc);
        return -1;
    }
    return 1;
}
