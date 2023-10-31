#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/string.h> // Add this header for basename function

#define KERNEL_UPSTREAM 0
#define TESTING 0
#define DEBUG 0

#if KERNEL_UPSTREAM==1
#include <linux/maple_tree.h>
#else
#include <linux/sched/mm.h>
#include <linux/slab.h>
#endif

#define DRIVER_NAME "TaxiDriver"
#define DRIVER
#include "../um/communication_struct.h"

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

#if TESTING == 1
static unsigned long translate_physical_to_virtual(unsigned long paddr) {
    unsigned long vaddr = 0xffff888000000000 | (paddr & (PAGE_SIZE - 1));
    return vaddr;
}
#endif

#if TESTING == 1
static unsigned long translate_virtual_to_physical(struct mm_struct *mm, unsigned long vaddr) {
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    unsigned long pfn;
    unsigned long paddr = 0;

    if (!mm)
        return 0; // Handle invalid mm_struct

    pgd = pgd_offset(mm, vaddr);
    if (!pgd_present(*pgd))
        return 0; // Handle invalid pgd

    p4d = p4d_offset(pgd, vaddr);
    if (!p4d_present(*p4d))
        return 0; // Handle invalid p4d

    pud = pud_offset(p4d, vaddr);
    if (!pud_present(*pud))
        return 0; // Handle invalid pud

    pmd = pmd_offset(pud, vaddr);
    if (!pmd_present(*pmd))
        return 0; // Handle invalid pmd

    pte = pte_offset_map(pmd, vaddr);
    if (!pte_present(*pte))
        return 0; // Handle invalid pte

    pfn = page_to_pfn(pte_page(*pte));
    paddr = (pfn << PAGE_SHIFT) | (vaddr & ~PAGE_MASK);
    pte_unmap(pte);

    return paddr;
}
#endif

int RPM(t_RPM args) {
    struct mm_struct *mm;
    unsigned long value = 0;
	
    printk(KERN_INFO "TaxiDriver: RPM --> addr : 0x%lx, size : %ld\n", args.addr, args.size);
    if (!args.addr)
	{
		return -1;
	}
    if (!task) {
        return -EINVAL;  // Invalid argument
    }
    mm = get_task_mm(task);
    if (mm) {
		if (access_process_vm(task, args.addr, &value, args.size, 0) == args.size) {
			printk(KERN_INFO "TaxiDriver: Value at 0x%lx: %lu\n", args.addr, value);
			mmput(mm);
		} else {
			printk(KERN_ALERT "TaxiDriver: Failed to read value at 0x%lx\n", args.addr);
			return -1;
		}
    }
    return (int)value;
}

int WPM(t_WPM args) {
    struct mm_struct *mm;
    int ret = 0;
	
    printk(KERN_INFO "TaxiDriver: WPM --> addr : 0x%lx, size : %ld, value : %ld\n",
	   args.addr, args.size, args.value);
    if (!args.addr)
        return -1;
    if (!task) {
        return -EINVAL;  // Invalid argument
    }
    mm = get_task_mm(task);
    if (mm) {
        if (access_process_vm(task, args.addr, &args.value, args.size, 1) == args.size) {
            printk(KERN_INFO "TaxiDriver: Successfully wrote value %lu to 0x%lx\n", args.value, args.addr);
            mmput(mm);
        } else {
            printk(KERN_ALERT "TaxiDriver: Failed to write value at 0x%lx\n", args.addr);
            ret = -1;
        }
    } else {
        ret = -EINVAL;
    }
    return ret;
}

static uintptr_t list_process_modules(const char *mod) {
	struct vm_area_struct *vma;
#if KERNEL_UPSTREAM == 1
    struct mm_struct *mm = task->mm;
    VMA_ITERATOR(vmi, mm, 0);

    for_each_vma(vmi, vma) {	
	if (vma->vm_file) {
            struct file *file = vma->vm_file;
            printk(KERN_INFO "TaxiDriver: Shared Library: %s start: 0x%lx end: 0x%lx\n",
		   file->f_path.dentry->d_name.name, vma->vm_start, vma->vm_end);
	    if (strcmp(file->f_path.dentry->d_name.name, mod) == 0)
		return (uintptr_t)vma->vm_start;
        }
    }
#else	
	struct task_struct *task;
	struct mm_struct *mm;
 
	for_each_process(task) {
		mm = get_task_mm(task);
 
		if (mm) {
			printk(KERN_INFO "\nTaxiDriver: PARENT PID: %d PROCESS: %s", task->pid, task->comm);
			down_read(&mm->mmap_lock);
			for (vma = mm->mmap; vma; vma = vma->vm_next) {
				printk(KERN_INFO "TaxiDriver: Address Range: 0x%lx - 0x%lx", vma->vm_start, vma->vm_end);
 
				if (vma->vm_file) {
					char buf[256];
					char *file_name = d_path(&vma->vm_file->f_path, buf, sizeof(buf));
					//const char *base_name = kbasename(file_name); // Extract the base name
					printk(KERN_INFO "TaxiDriver: Mapped File: %s", file_name);
					//printk(KERN_INFO "TaxiDriver: Mapped File: %s", base_name);
					if (strcmp(file_name, mod) == 0)
					//if (strcmp(base_name, mod) == 0) // Compare with the base name
						//printk(KERN_INFO "TaxiDriver: Mapped File %s found at 0x%lx - 0x%lx", base_name, vma->vm_start, vma->vm_end);
						return (uintptr_t)vma->vm_start;
				}
			}
 
			up_read(&mm->mmap_lock);
			mmput(mm);
		}
	}
#endif
	return 0;
}

//only checked for KERNEL_UPSTREAM == 0
#if KERNEL_UPSTREAM == 0
static unsigned long long list_pid_mod(t_PM args) {
	printk(KERN_INFO "\nTaxiDriver list_pid_mod:%s", args.mod);
	struct task_struct *task;
	struct mm_struct *mm;
	struct vm_area_struct *vma;
 
	for_each_process(task) {
		mm = get_task_mm(task);
 
		if (mm) {
			if(task->pid == args.pid)
			{
				printk(KERN_INFO "\nTaxiDriver: PARENT PID: %d PROCESS: %s", task->pid, task->comm);
			}
			down_read(&mm->mmap_lock);
			for (vma = mm->mmap; vma; vma = vma->vm_next) {
				//printk(KERN_INFO "TaxiDriver: Address Range: 0x%lx - 0x%lx", vma->vm_start, vma->vm_end);
				if (vma->vm_file) {
					char buf[256];
					char *file_name = d_path(&vma->vm_file->f_path, buf, sizeof(buf));
					const char *base_name = kbasename(file_name);
					if (strcmp(base_name, args.mod) == 0)
					{
						unsigned long long vmstartaddr = (unsigned long long)vma->vm_start;
						if(vmstartaddr>0)
						{
							printk(KERN_INFO "TaxiDriver: Mapped File %s found at 0x%lx - 0x%lx", base_name, vma->vm_start, vma->vm_end);
							return vmstartaddr;
						}
					}
				}
			}
			up_read(&mm->mmap_lock);
			mmput(mm);
		}
	}
	return 0;
}
#endif

static int init_process_by_pid(int target_pid) {
    printk(KERN_INFO "TaxiDriver: Accessing process with PID: %d\n", target_pid);

    struct pid *pid_struct;
    pid_struct = find_get_pid(target_pid);
    if (pid_struct) {
        task = get_pid_task(pid_struct, PIDTYPE_PID);
        if (task) {
            const char *process_name = task->comm;
            printk(KERN_INFO "TaxiDriver: Process with PID %d has name: %s\n", target_pid, process_name);
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
#if KERNEL_UPSTREAM == 0
	struct s_PM pm_args;
#endif
    const char *mod = kmalloc(sizeof(char) * 256, GFP_KERNEL);
    if (!mod)
	{
		return -ENOMEM;
	}

    static uintptr_t addr = 0;
    int pid;
    long return_value = 0;
	
    switch (ioctl_num) {
        case IOCTL_GETMODULE:
            if (copy_from_user((void *)mod, (int *)arg, sizeof(char) * 256))
                return -EFAULT;
			return_value = list_process_modules(mod);
			addr = return_value;
			kfree(mod);
            break;
#if KERNEL_UPSTREAM == 0
		case IOCTL_GETPIDMODULE:
			if (copy_from_user(&pm_args, (int *)arg, sizeof(t_PM)))
				return -EFAULT;
			
			if (copy_from_user(mod, pm_args.mod, 256)) // ptr/data Userspace to Kernel else perms/page fault
			{
				return -EFAULT;
			}
			pm_args.mod = mod;
			
			return_value = list_pid_mod(pm_args);
			addr = return_value;
			kfree(mod);
			break;
#endif
        case IOCTL_OPENPROC:
            if (copy_from_user(&pid, (int *)arg, sizeof(int)))
                return -EFAULT;
			return_value = init_process_by_pid(pid);
            break;

        case IOCTL_RPM:
            if (copy_from_user(&rpm_args, (int *)arg, sizeof(t_RPM)))
                return -EFAULT;
			if (rpm_args.addr == 0x69420) {
				put_user(addr, rpm_args.out_addr);
				break;
			}
			return_value = RPM(rpm_args);
			put_user(return_value, rpm_args.out_addr);
            break;

        case IOCTL_WPM:
            if (copy_from_user(&wpm_args, (int *)arg, sizeof(t_WPM)))
                return -EFAULT;
			return_value = WPM(wpm_args);
            break;
#if KERNEL_UPSTREAM == 0 && TESTING==1
        case IOCTL_VIRT_TO_PHYS:
            {
                struct mm_struct *mm = current->mm; // Get the current process's mm_struct
                unsigned long vaddr;
                unsigned long paddr;

                if (copy_from_user(&vaddr, (unsigned long *)arg, sizeof(unsigned long))) {
                    return -EFAULT;
                }

                paddr = translate_virtual_to_physical(mm, vaddr);
                return_value = paddr;
                if (copy_to_user((unsigned long *)arg, &return_value, sizeof(unsigned long))) {
                    return -EFAULT;
                }
            }
            break;
#endif

#if KERNEL_UPSTREAM == 0 && TESTING==1
        case IOCTL_PHYS_TO_VIRT:
            {
                unsigned long paddr;
                unsigned long vaddr;

                if (copy_from_user(&paddr, (unsigned long *)arg, sizeof(unsigned long))) {
                    return -EFAULT;
                }

                vaddr = translate_physical_to_virtual(paddr);
                if (copy_to_user((unsigned long *)arg, &vaddr, sizeof(unsigned long))) {
                    return -EFAULT;
                }
            }
            break;
#endif

        default:
            return -ENOTTY;
    }
#if KERNEL_UPSTREAM == 0 && TESTING==1
	printk(KERN_INFO "return_value=0x%llx\n", (unsigned long long)return_value);
#endif
    return return_value;
}

static struct file_operations fops = {
    .unlocked_ioctl = device_ioctl,
    .open = device_open,
    .release = device_release,
};

static int __init driver_init(void)
{
    printk(KERN_INFO "TaxiDriver: Loaded\n");

    // Dynamically allocate the major number
    major_number = register_chrdev(0, DRIVER_NAME, &fops);

    if (major_number < 0) {
        printk(KERN_ALERT "TaxiDriver: Failed to register the driver.\n");
        return major_number;
    }

    printk(KERN_INFO "TaxiDriver: Registered %s with major number %d\n", DRIVER_NAME, major_number);

    return 0;
}

static void __exit driver_exit(void)
{
    unregister_chrdev(major_number, DRIVER_NAME);
    printk(KERN_INFO "TaxiDriver: Unloaded\n");
}

module_init(driver_init);
module_exit(driver_exit);
MODULE_LICENSE("GPL");
