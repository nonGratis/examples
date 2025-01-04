
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/moduleparam.h>

MODULE_AUTHOR("Andrii Shapovalov <shapovalov.andrii@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

/* Визначаємо параметри */
static uint print_count = 1; /* Наше звичайне значення */
module_param(print_count, uint, 0444);
MODULE_PARM_DESC(print_count, "Number of times to print 'Hello, world!'");

/* Встановлюємо стуртуру елементів */
struct hello_entry {
	struct list_head list;
	ktime_t time;
};

/* Оголошуємо глобально */
static LIST_HEAD(hello_list);

/* Ініціалізуємо модуль */
static int __init hello_init(void)
{
	uint i; /* Ініціалізуємо змінну для циклу */
	struct hello_entry *entry;

	pr_info("Hello module loading with print_count=%u\n", print_count);

    /* Та необхідна валідація */
	if (print_count == 0 || (print_count >= 5 && print_count <= 10)) {
		pr_warn("Warning: print_count is in the range of 0, 5-10.\n");
	} else if (print_count > 10) {
		pr_err("Error: print_count is greater than 10. Exiting with -EINVAL.\n");
		return -EINVAL;
	}

    /* Друк повідомлень і запис часу */
	for (i = 0; i < print_count; i++) {
		entry = kmalloc(sizeof(*entry), GFP_KERNEL);
		if (!entry)
			return -ENOMEM;
		entry->time = ktime_get();
		list_add_tail(&entry->list, &hello_list);

		pr_emerg("Hello, world! Time: %llu ns\n", entry->time);
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_entry *entry, *tmp;

	pr_info("Hello module unloading.\n");

    /* Звільняємо списки */
	list_for_each_entry_safe(entry, tmp, &hello_list, list) {
		pr_info("Time: %llu ns\n", entry->time);
		list_del(&entry->list);
		kfree(entry);
	}
}

module_init(hello_init);
module_exit(hello_exit);
