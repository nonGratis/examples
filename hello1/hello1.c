// SPDX-License-Identifier: BSD/GPL

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
#include "hello1.h"

MODULE_AUTHOR("Andrii Shapovalov <shapovalov.andrii@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello1 Module - Exports print_hello()");
MODULE_LICENSE("Dual BSD/GPL");

/* Встановлюємо стуртуру елементів */
struct hello_entry {
	struct list_head list;
	ktime_t time;
};

/* Оголошуємо глобально */
static LIST_HEAD(hello_list);

void print_hello(uint count)
{
	uint i;
	struct hello_entry *entry;

	BUG_ON(count == 0); // Додаємо BUG_ON для недійсних параметрів

	for (i = 0; i < count; i++) {
		if (i == 4) { // Примусова помилку kmalloc на 5-й ітерації
			entry = NULL;
		} else {
			entry = kmalloc(sizeof(*entry), GFP_KERNEL);
		}
		if (!entry)
			return;
		entry->time = ktime_get();
		list_add_tail(&entry->list, &hello_list);
		pr_info("Hello, world! Time: %llu ns\n", entry->time);
	}
}
EXPORT_SYMBOL(print_hello);

/* Функція виходу з модуля */
static void __exit hello1_exit(void)
{
	struct hello_entry *entry, *tmp;

	pr_info("Unloading hello1 module.\n");

	list_for_each_entry_safe(entry, tmp, &hello_list, list) {
		pr_info("Time: %llu ns\n", entry->time);
		list_del(&entry->list);
		kfree(entry);
	}
}

module_exit(hello1_exit);
