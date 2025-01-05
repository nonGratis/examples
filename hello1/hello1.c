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
 *	must display the following acknowledgement:
 *	This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *	names of its contributors may be used to endorse or promote products
 *	derived from this software without specific prior written permission.
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
#include <linux/bug.h>

MODULE_AUTHOR("Andrii Shapovalov <shapovalov.andrii@lll.kpi.ua>");
MODULE_DESCRIPTION("Hello1 Module - Exports print_hello()");
MODULE_LICENSE("Dual BSD/GPL");

static uint print_count = 1;
module_param(print_count, uint, 0444);
MODULE_PARM_DESC(print_count, "Number of times to print hello");

/* Встановлюємо стуртуру елементів */
struct hello_entry {
	struct list_head list;
	ktime_t time;
};

/* Оголошуємо глобально */
static LIST_HEAD(hello_list);

// Function prototype
void print_hello(uint count);

static int __init hello1_init(void)
{
    BUG_ON(print_count > 10);
    print_hello(print_count);
    return 0;
}

void print_hello(uint count)
{
	uint i;
	struct hello_entry *entry;

	if (count == 0 || (count > 4 && count < 11))
	pr_warn("WARNING: %u elements requested, but should be 1-4\n",
		count);

	for (i = 0; i < count; i++) {
		if (i == 4) // Примусова kmalloc в 5 ітерації
			entry = NULL;
		else
			entry = kmalloc(sizeof(*entry), GFP_KERNEL);

		if (!entry) {
			pr_err("Failed to allocate memory\n");
			continue;
		}

		entry->time = ktime_get();
		list_add_tail(&entry->list, &hello_list);
		pr_emerg("Hello, world! Time: %llu ns\n", entry->time);
	}
}
EXPORT_SYMBOL(print_hello);

/* Функція виходу з модуля */
static void __exit hello1_exit(void)
{
	struct hello_entry *entry, *temp;

	list_for_each_entry_safe(entry, temp, &hello_list, list) {
		list_del(&entry->list);
		kfree(entry);
	}
}

module_init(hello1_init);
module_exit(hello1_exit);