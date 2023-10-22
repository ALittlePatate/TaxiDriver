#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "TaxiDriver"
#define DRIVER
#include "client/communication_struct.h"

static int major_number;
static struct task_struct *task;

static int device_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    return 0;
}

int RPM(t_RPM args) {
    struct mm_struct *mm;
    unsigned long value = 0;

    printk(KERN_ALERT "TaxiDriver: RPM --> addr : 0x%lx, size : %ld\n", rpm_args.addr, rpm_args.size);
    if (args.addr == 0)
	return -1;
    mm = get_task_mm(task);
    if (mm != NULL) {
        if (access_process_vm(task, args.addr, &value, args.size, 0) == args.size) {
	    printk(KERN_INFO "TaxiDriver: Value at 0x%lx: %lu\n", args.addr, value);
	    mmput(mm);
	} else {
	    printk(KERN_INFO "TaxiDriver: Failed to read value at 0x%lx\n", args.addr);
	    return -1;
	}
    }
    return (int)value;
}

static int init_process_by_pid(int target_pid) {
    printk(KERN_INFO "TaxiDriver: Accessing process with PID: %d\n", target_pid);

    struct pid *pid_struct;
    pid_struct = find_get_pid(target_pid);
    if (pid_struct != NULL) {
        task = get_pid_task(pid_struct, PIDTYPE_PID);
        if (task != NULL) {
            const char *process_name = task->comm;
            printk(KERN_INFO "TaxiDriver: Process with PID %d has name: %s\n", target_pid, process_name);
            // Access and manipulate the process here
            put_task_struct(task);
        } else {
            printk(KERN_INFO "TaxiDriver: Process with PID %d not found\n", target_pid);
	    return -1;
        }
        put_pid(pid_struct);
    } else {
        printk(KERN_INFO "TaxiDriver: Process with PID %d not found\n", target_pid);
	return -1;
    }

    return 1;  // A successful module initialization
}

static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long arg)
{
    struct s_WPM wpm_args;
    struct s_RPM rpm_args;
    int pid;
    int return_value = 0;

    switch (ioctl_num) {
        case IOCTL_OPENPROC:
            if (copy_from_user(&pid, (int *)arg, sizeof(int)))
                return -EFAULT;
	    return_value = init_process_by_pid(pid);
            break;

        case IOCTL_RPM:
            if (copy_from_user(&rpm_args, (int *)arg, sizeof(t_RPM)))
                return -EFAULT;
	    return_value = RPM(rpm_args);
            break;

        case IOCTL_WPM:
            if (copy_from_user(&wpm_args, (int *)arg, sizeof(t_WPM)))
                return -EFAULT;
	    printk(KERN_ALERT "TaxiDriver: WPM --> addr : %ld, size : %ld, value : %ld\n",
		   wpm_args.addr, wpm_args.size, wpm_args.value);
	    return_value = 1337;
            break;

        default:
            return -ENOTTY;
    }

    return return_value;
}

static struct file_operations fops = {
    .unlocked_ioctl = device_ioctl,
    .open = device_open,
    .release = device_release,
};

static int __init driver_init(void)
{
    printk(KERN_ALERT "TaxiDriver: Loaded\n");

    // Dynamically allocate the major number
    major_number = register_chrdev(0, DRIVER_NAME, &fops);

    if (major_number < 0) {
        printk(KERN_ALERT "TaxiDriver: Failed to register the driver.\n");
        return major_number;
    }

    printk(KERN_ALERT "TaxiDriver: Registered %s with major number %d\n", DRIVER_NAME, major_number);

    return 0;
}

static void __exit driver_exit(void)
{
    unregister_chrdev(major_number, DRIVER_NAME);
    printk(KERN_ALERT "TaxiDriver: Unloaded\n");
}

module_init(driver_init);
module_exit(driver_exit);
MODULE_LICENSE("GPL");
