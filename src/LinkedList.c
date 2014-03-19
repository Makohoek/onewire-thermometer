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
#include "LinkedList.h"
static const unsigned char VERBOSE = 0;

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

static void addListToLinkedList(LinkedList* theLinkedListToWrite, ITEM_TYPE_LL item);
ITEM_TYPE_LL getItemFromIndex(LinkedList* theLinkedListToRead, unsigned int index);

// util functions
static int isAddingFirstCell(LinkedCell* lastCell);

LinkedList *newLinkedList(void)
{
  LinkedList* theNewLinkedList = MALLOC(sizeof(LinkedList));

    if (theNewLinkedList == NULL)
    {
      PRINT("Error when allocating memory for linked list\n");
      return NULL;
    }

  theNewLinkedList->numberOfItems = 0;
  theNewLinkedList->first = NULL;
  theNewLinkedList->last = NULL;

  /* function pointers for OO behaviour */
  theNewLinkedList->writeItem = addListToLinkedList;
  theNewLinkedList->getItemFromIndex = getItemFromIndex;

  return theNewLinkedList;
}

void deleteLinkedList(LinkedList *theLinkedListToDelete)
{
  if (theLinkedListToDelete == NULL)
  {
    PRINT("Attempt to destroy a NULL linked list\n");
    return;
  }

  /* deleteAllCells */
  while (theLinkedListToDelete->first != NULL)
  {
    LinkedCell* currentCellToDestroy = theLinkedListToDelete->first;
    theLinkedListToDelete->first = theLinkedListToDelete->first->next;

    FREE(currentCellToDestroy->item);
    FREE(currentCellToDestroy);
    currentCellToDestroy = NULL;
  }

  FREE(theLinkedListToDelete);
}

ITEM_TYPE_LL getItemFromIndex(LinkedList* theLinkedListToRead, unsigned int index)
{
  unsigned int i = 0;
  LinkedCell* currentCell = NULL;

  if (theLinkedListToRead == NULL)
  {
    PRINT("getItemFromIndex(): ERROR: item == NULL\n");
    return DEFAULT_ITEM;
  }

  currentCell = theLinkedListToRead->first;
  while (currentCell != NULL && i <= index)
  {
    if (i == index)
    {
      return currentCell->item;
    }
    currentCell = currentCell->next;
    ++i;
  }
  PRINT("ERROR: no item with this index");
  return DEFAULT_ITEM;
}

static void addListToLinkedList(LinkedList* theLinkedListToWrite, ITEM_TYPE_LL item)
{
  LinkedCell* newCellToAdd = MALLOC(sizeof(struct _linkedCell));
  LinkedCell* lastCell = NULL;

  if (newCellToAdd == NULL)
  {
    PRINT("Error when allocating memory for a new buffer\n");
    return;
  }

  newCellToAdd->item = item;
  newCellToAdd->next = NULL;

  lastCell = theLinkedListToWrite->last;
  if (isAddingFirstCell(lastCell))
  {
    // adding first element
    theLinkedListToWrite->first = newCellToAdd;
    theLinkedListToWrite->last = newCellToAdd;
  }
  else
  {
    lastCell->next = newCellToAdd;
    theLinkedListToWrite->last = newCellToAdd;
  }

  theLinkedListToWrite->numberOfItems++;
}

static int isAddingFirstCell(LinkedCell* lastCell)
{
  return lastCell == NULL;
}
