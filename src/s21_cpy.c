#include "s21_string.h"

char* s21_strcpy(char* restrict destination, const char* restrict source) {
  if (destination == NULL) {
    return NULL;
  }

  char* ptr = destination;  // pointer to the dst start

  while (*source != '\0') {
    *destination = *source;
    destination++;
    source++;
  }
  *destination = '\0';

  return ptr;
}