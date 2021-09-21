#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
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
__used __section(__versions) = {
	{ 0x573c539f, "module_layout" },
	{ 0x5727cd8a, "class_destroy" },
	{ 0x693f98d7, "device_destroy" },
	{ 0x58527aba, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x4bcb110d, "__class_create" },
	{ 0x2edc1bb5, "__register_chrdev" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xa916b694, "strnlen" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x1d19f77b, "physical_mask" },
	{ 0x22238da7, "kernel_write" },
	{ 0xef502ce7, "kernel_read" },
	{ 0x9907994e, "walk_page_range_novma" },
	{ 0xc5850110, "printk" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0x56511cd1, "filp_close" },
	{ 0x8c74e768, "pid_task" },
	{ 0xb404ec0e, "find_get_pid" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x4205b677, "filp_open" },
	{ 0xce87c7bf, "current_task" },
	{ 0x2da9a0bd, "pv_ops" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "62BF75F563E466D1A8CFED5");
