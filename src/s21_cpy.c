#include "s21_string.h"

// void *memcpy(void *dest, const void *src, size_t n)
// void *memmove(void *dest, const void *src, size_t n)
// void *memset(void *str, int c, size_t n)
// char *strcpy(char *dest, const char *src)
// char *strncpy(char *dest, const char *src, size_t n)

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