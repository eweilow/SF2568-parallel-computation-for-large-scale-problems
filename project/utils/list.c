#include <string.h>

#ifndef DEBUG_LIST
  #define DEBUG_LIST 0
#endif

typedef struct {
  void* memptr;
  size_t elementSize;
  size_t elementCount;
  size_t currentMemorySize;
} List;

List initList(
  size_t elementSize,
  size_t initialAllocation
) {
  List list;

  list.elementSize = elementSize;
  list.elementCount = 0;

  list.currentMemorySize = elementSize * initialAllocation;

  if(list.currentMemorySize > 0) {
    list.memptr = (void*)malloc(elementSize * initialAllocation);
  } else {
    list.memptr = NULL;
  }

  #if DEBUG_LIST_ALLOC
    printf("[LIST] Allocating list. size: %ld, initial count: %ld\n", elementSize, initialAllocation);
  #endif

  return list;
}

long list_memSize(
  List* list,
  size_t elementCount
) {
  return list->elementSize * elementCount;
}

// https://jameshfisher.com/2018/03/30/round-up-power-2/
long next_pow2(long x) {
  long p = 1;
  while (p < x) p <<= 1;
  return p;
}

void list_increaseSize(
  List* list,
  size_t newSize
) {
  long newSizePower2 = next_pow2(newSize);

  if(newSizePower2 < list->currentMemorySize) {
    #if DEBUG_LIST_ALLOC
      printf("[LIST] Skipping size increase from %ld elements (%ld bytes) to %ld elements (%ld bytes)\n", list->currentMemorySize/list->elementSize, list->currentMemorySize, newSizePower2 / list->elementSize, newSizePower2);
    #endif
  } else {
    #if DEBUG_LIST_ALLOC
      printf("[LIST] Increasing size from %ld elements (%ld bytes) to %ld elements (%ld bytes)\n", list->currentMemorySize/list->elementSize, list->currentMemorySize, newSizePower2 / list->elementSize, newSizePower2);
    #endif
    list->memptr = (void*) realloc(list->memptr, newSizePower2);
    list->currentMemorySize = newSizePower2;
  }
}

void list_decreaseSize(
  List* list,
  size_t newSize
) {
  long newSizePower2 = newSize == 0 ? 0 : next_pow2(newSize);
  if(newSizePower2 == 1) {
    #if DEBUG_LIST
      printf("[LIST] Skipping list clear (new size too small) after moving from %ld elements (%ld bytes) to %ld elements (%ld bytes)\n", list->currentMemorySize/list->elementSize, list->currentMemorySize, newSizePower2 / list->elementSize, newSizePower2);
    #endif
    return;
  }
  #if DEBUG_LIST_ALLOC
    printf("[LIST] Decreasing size from %ld elements (%ld bytes) to %ld elements (%ld bytes)\n", list->currentMemorySize/list->elementSize, list->currentMemorySize, newSizePower2 / list->elementSize, newSizePower2);
  #endif
  list->memptr = (void*) realloc(list->memptr, newSizePower2);
  list->currentMemorySize = newSizePower2;
}

void list_insert(
  List* list,
  void* element
) {
  size_t curSize = list->currentMemorySize;
  size_t necessarySize = list_memSize(list, list->elementCount + 1);
  #if DEBUG_LIST
    printf("[LIST] Inserting into list: curSize: %ld, necessarySize: %ld\n", curSize, necessarySize);
  #endif

  if(curSize < necessarySize) {
    list_increaseSize(list, necessarySize);
  }

  long index = list->elementCount;
  long byteIndex = index * list->elementSize;
  #if DEBUG_LIST
    printf("[LIST] Writing at location %ld (%ld bytes)\n", index, byteIndex);
  #endif
  memcpy(list->memptr + byteIndex, element, list->elementSize);
  list->elementCount = list->elementCount + 1;
}

void list_remove(
  List* list,
  long index,
  void* deleted
) {
  if(index < 0) {
    #if DEBUG_LIST
      printf("[LIST] Skipping remove: index %d < 0\n", index);
    #endif
    return;
  }
  if(index >= list->elementCount) {
    #if DEBUG_LIST
      printf("[LIST] Skipping remove: index %d > count (%d)\n", index, list->elementCount);
    #endif
    return;
  }

  long copyFromIndex = index + 1;
  long copyToIndex = index;

  long copyFromByteIndex = copyFromIndex * list->elementSize;
  long copyToByteIndex = copyToIndex * list->elementSize;

  long length = list->elementCount - copyFromIndex;
  long byteLength = length * list->elementSize;

  #if DEBUG_LIST
    printf("[LIST] Removing element at %ld (@%ld bytes)\n", 
      copyToIndex, copyToByteIndex
    );
    printf("[LIST] Moving %ld elements (%ld bytes) down one index from %ld (@%ld bytes) to %ld (@%ld bytes)\n", 
      length, byteLength, 
      copyFromIndex, copyFromByteIndex,
      copyToIndex, copyToByteIndex
    );
  #endif
  
  memcpy(deleted, list->memptr + copyToByteIndex, list->elementSize);

  memmove(list->memptr + copyToByteIndex, list->memptr + copyFromByteIndex, byteLength);
  list->elementCount = list->elementCount - 1;

  size_t newMemorySize = list_memSize(list, list->elementCount);
  if(newMemorySize <= list->currentMemorySize / 2) {
    list_decreaseSize(list, newMemorySize);
  }
}

void list_count(
  List* list,
  size_t* count
) {
  *count = list->elementCount;
}

void list_read(
  List* list,
  size_t* count,
  void** ptr
) {
  list_count(list, count);
  *ptr = list->memptr;
}


void list_clear(
  List* list
) {
  if(list->currentMemorySize > 0) {
    free(list->memptr);
    list->memptr = NULL;
    list->currentMemorySize = 0;
  }
  list->elementCount = 0;
}

List list_clone(
  List* list
) {
  if(DEBUG_LIST) {
    printf("[LIST] Copying list of size %ld (%ld bytes)\n", 
      list->elementCount,
      list->currentMemorySize
    );
  }
  List cloned;

  cloned.memptr = (void*)malloc(list->currentMemorySize);
  memcpy(cloned.memptr, list->memptr, list->currentMemorySize);
  
  cloned.elementSize = list->elementSize;
  cloned.elementCount = list->elementCount;
  cloned.currentMemorySize = list->currentMemorySize;

  return cloned;
}

long lengthOfList(
  List* list
) {
  long count;
  list_count(list, &count); // get count of foxes into count
  return count;
}

long randomIndexOfList(
  List* list
) {
  return getRandomInt(lengthOfList(list));
}