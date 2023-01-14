#include "s21_string.h"

// void *memcpy(void *dest, const void *src, size_t n)
// void *memmove(void *dest, const void *src, size_t n)
// void *memset(void *str, int c, size_t n)
// char *strcpy(char *dest, const char *src)
// char *strncpy(char *dest, const char *src, size_t n)

char* s21_strncpy(char* dest, const char* src, size_t n) {
  size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++) {
    dest[i] = src[i];
  }
  for (; i < n; i++) {
    dest[i] = '\0';
  }

  return dest;
}

char* s21_strcpy(char* restrict dest, const char* restrict src) {
  return s21_strncpy(dest, src, s21_strlen(src));
}