/**
 * Copyright (c) 2014, Mattijs Korpershoek
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */
#ifndef __DEFINE_LINKEDLISTLINKEDLIST__
#define __DEFINE_LINKEDLISTLINKEDLIST__

#define KERNEL_DEVELOPMENT 1

/* set right macros here for hybrid kernel/userspace usage */
#if KERNEL_DEVELOPMENT == 1

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include "SensorID.h"

#define FREE kfree
#define MALLOC(x) kmalloc(x, GFP_KERNEL)
#define PRINT(x) printk (KERN_ALERT x)
#define ITEM_TYPE_LL Sensor*
#define DEFAULT_ITEM NULL

#else
#include <stdio.h>
#include <stdlib.h>

#define FREE free
#define MALLOC(x) malloc(x)
#define PRINT(x) printf("%s", x)
#define ITEM_TYPE char*
#define DEFAULT_ITEM NULL

#endif

/* cell structure */
typedef struct _linkedCell
{
  struct _linkedCell* next;
  ITEM_TYPE_LL item;
}LinkedCell;

typedef struct _internalLinkedList
{
  LinkedCell* first;
  LinkedCell* last;
  unsigned long numberOfItems;
  void (*writeItem)(struct _internalLinkedList*, ITEM_TYPE_LL);
  ITEM_TYPE_LL (*getItemFromIndex)(struct _internalLinkedList*, unsigned int index);
}LinkedList;

LinkedList* newLinkedList(void);
void deleteLinkedList(LinkedList* theLinkedListToDelete);

#endif
