/*
 * LinkedList.c
 *
 * Mattijs KORPERSHOEK
 * <mattijs.korpershoek@gmail.com>
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
