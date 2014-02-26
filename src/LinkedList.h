/**
 * LinkedList.h
 *
 *	Mattijs KORPERSHOEK
 *	<mattijs.korpershoek@gmail.com>
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
#include "GlobalData.h"
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
