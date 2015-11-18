#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x59caa4c3, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x5dc6ed13, __VMLINUX_SYMBOL_STR(xt_unregister_targets) },
	{ 0xcbf1a9e4, __VMLINUX_SYMBOL_STR(xt_register_targets) },
	{ 0xa74bc011, __VMLINUX_SYMBOL_STR(kfree_skb) },
	{ 0xb30280b6, __VMLINUX_SYMBOL_STR(dst_release) },
	{ 0x49bf462c, __VMLINUX_SYMBOL_STR(__pskb_pull_tail) },
	{ 0x61b63e58, __VMLINUX_SYMBOL_STR(ip_local_out) },
	{ 0x4aa88c98, __VMLINUX_SYMBOL_STR(nf_ct_attach) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xb6b46bf4, __VMLINUX_SYMBOL_STR(skb_push) },
	{ 0x1172c8f, __VMLINUX_SYMBOL_STR(__alloc_skb) },
	{ 0x1fbd16da, __VMLINUX_SYMBOL_STR(ip_tos2prio) },
	{ 0xe2efc359, __VMLINUX_SYMBOL_STR(ip_route_output_flow) },
	{ 0xbe7b5971, __VMLINUX_SYMBOL_STR(init_net) },
	{ 0x5ea51195, __VMLINUX_SYMBOL_STR(skb_pull) },
	{ 0xf0fdf6cb, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x4f68e5c9, __VMLINUX_SYMBOL_STR(do_gettimeofday) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=x_tables";


MODULE_INFO(srcversion, "8E191097500D7BCE9231693");