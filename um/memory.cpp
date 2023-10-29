#include "memory.hpp"

int file_desc = 0;
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

int get_pid(const char *program_name) {
    FILE *fp;
    char command[128];
    char buffer[128];
    int pid = -1;

    // Create a command to run 'pidof' for the specified program
    snprintf(command, sizeof(command), "pidof %s", program_name);

    // Open a pipe to execute the command and read the output
    fp = popen(command, "r");
    if (!fp) {
        perror("popen");
	return -1;
    }

    // Read the output (should be a space-separated list of PIDs)
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Extract the first PID from the list
        if (sscanf(buffer, "%d", &pid) == 1) {
        }
    }

    // Close the pipe and check for errors
    if (pclose(fp) == -1) {
        perror("pclose");
	return -1;
    }

    return pid;
}

int open_process(int pid)
{
    uint64_t ret = ioctl(file_desc, IOCTL_OPENPROC, &pid);
    if (ret < 0) {
        perror("Revird: openprocess failed.");
        close(file_desc);
        return -1;
    }
    return 1;
}

uintptr_t get_module(const char *mod)
{
    uint64_t ret = ioctl(file_desc, IOCTL_GETMODULE, mod);
    if (ret < 0) {
        perror("Revird: getmodule failed.");
        close(file_desc);
        return -1;
    }

    uintptr_t addr = RPM<uintptr_t>(0x69420);
    return addr;
}

#if KERNEL_UPSTREAM == 0
uintptr_t get_pid_module(int pid, const char *mod) {
    struct s_PM pma;
    pma.pid = pid;
    pma.mod = mod;
	
    uint64_t ret = ioctl(file_desc, IOCTL_GETPIDMODULE, &pma);
    if (ret < 0) {
        perror("Revird: get_pid_module failed.");
        close(file_desc);
        return -1;
    }

    uintptr_t addr = RPM<uintptr_t>(0x69420);
    return addr;
}
#endif

#if KERNEL_UPSTREAM == 0 && TESTING==1
uintptr_t VIRT_TO_PHYS(uintptr_t vaddr) {
    uint64_t ret = ioctl(file_desc, IOCTL_VIRT_TO_PHYS, vaddr);
    if (ret < 0) {
        perror("VIRT_TO_PHYS failed.");
        return 0;
    }
    // At this point, 'vaddr' contains the physical address.
    return vaddr;
}
#endif

#if KERNEL_UPSTREAM == 0 && TESTING==1
uintptr_t PHYS_TO_VIRT(uintptr_t paddr) {
    uint64_t ret = ioctl(file_desc, IOCTL_PHYS_TO_VIRT, &paddr);
    if (ret < 0) {
        perror("PHYS_TO_VIRT failed.");
        return 0;
    }

    // At this point, 'paddr' contains the virtual address.
    return paddr;
}
#endif
