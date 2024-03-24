// Hooked reboot and kill syscall
static int handler_pre_kill(struct kprobe *p, struct pt_regs *regs)
{
    pid_t pid = regs->regs[1];
    printk(KERN_INFO "Pre-handler: kill syscall intercepted. PID: %d\n", pid);
    return 0;
}

static void handler_post_kill(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
    printk(KERN_INFO "Post-handler: kill executed.\n");
}

static int handler_pre_reboot(struct kprobe *p, struct pt_regs *regs)
{
    // int magic1 = regs->regs[0];
    // int magic2 = regs->regs[1];
    // unsigned int cmd = regs->regs[2];
    // void __user *arg = (void __user *)regs->regs[3];
    printk(KERN_INFO "Pre-handler: reboot syscall intercepted.\n");
    return 0;
}

static void handler_post_reboot(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
    printk(KERN_INFO "Post-handler: reboot executed.\n");
}

static struct kprobe kp_kill = {
    .symbol_name = "__arm64_sys_kill",
    .pre_handler = handler_pre_kill,
    .post_handler = handler_post_kill,

};

static struct kprobe kp_reboot = {
    .symbol_name = "__arm64_sys_reboot",
    .pre_handler = handler_pre_reboot,
    .post_handler = handler_post_reboot,

};