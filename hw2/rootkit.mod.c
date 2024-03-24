#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x87dbb23e, "module_layout" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x42defd9f, "cdev_del" },
	{ 0xcbae9d32, "cdev_add" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xeb78b1ed, "unregister_kprobe" },
	{ 0x472cf3b, "register_kprobe" },
	{ 0x85708c7, "cdev_alloc" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x37a0cba, "kfree" },
	{ 0x4829a47e, "memcpy" },
	{ 0x98cf60b3, "strlen" },
	{ 0xa916b694, "strnlen" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x912dfc99, "init_task" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0xdcb764ad, "memset" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xb788fb30, "gic_pmr_sync" },
	{ 0x908e5601, "cpu_hwcaps" },
	{ 0x69f38847, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x92997ed8, "_printk" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "18FD7552BD8FA943E29C137");
