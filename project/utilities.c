#include "time.h"
#include <stdlib.h>
#include <stdio.h>

#define for_each_item(item, list) \
    for(T * item = list->head; item != NULL; item = item->next)


long getRandomInt(long upperBound) {
  return rand() * upperBound / RAND_MAX;
}

bool arrayContains(long* array, long number) {
  if (array == NULL) {
    perror("attempted to see if null-array contains number");
    return false;
  }

  for (int i=0; i<sizeof(array);i++) {
    if (array[i] == number)
      return true;

  return false;
  }
}

long* getRandomNumbers(long nNumbers, long upperBound){
  long randomNumbers[nNumbers];
  int i = 0;
  while (i < nNumbers){
    long draw = getRandomInt(upperBound);
    if (arrayContains(randomNumbers, draw)) {
      // do nothing, wait for next draw
    } else {
      randomNumbers[i] = draw;
      i++;
    }
  }
}
