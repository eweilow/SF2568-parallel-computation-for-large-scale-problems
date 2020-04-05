#include "time.h"
#include <stdlib.h>

#define for_each_item(item, list) \
    for(T * item = list->head; item != NULL; item = item->next)



long getRandomInt(long upperBound) {
  srand(time(NULL));   // Initialization, should only be called once.
  long r = rand();
  return r;
}
