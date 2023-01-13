#include "s21_string.h"

// int memcmp(const void *str1, const void *str2, size_t n)
// int strcmp(const char *str1, const char *str2)
// int strncmp(const char *str1, const char *str2, size_t n)

int s21_strcmp(const char* first, const char* second) {
  while (*first != '\0' && *first == *second) {
    ++first;
    ++second;
  }

  return *(const unsigned char*)first - *(const unsigned char*)second;
}