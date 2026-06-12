#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

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
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xfe990052, "gpio_free" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xc82502b7, "gpio_to_desc" },
	{ 0x32c86573, "gpiod_set_raw_value" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x8dd1ea23, "gpiod_direction_output_raw" },
	{ 0x47d6e737, "gpiod_direction_input" },
	{ 0x591afd64, "gpiod_to_irq" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xf30e3bab, "class_create" },
	{ 0xcec68e0, "device_create" },
	{ 0x7fa130a5, "cdev_init" },
	{ 0xcdf93547, "cdev_add" },
	{ 0xfa2c8345, "pcpu_hot" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0xf9a482f9, "msleep" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x80915c52, "cdev_del" },
	{ 0xb19cbd09, "device_destroy" },
	{ 0xe87a29e0, "class_destroy" },
	{ 0x7bec7de9, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "CFA3648947129E88467A308");
