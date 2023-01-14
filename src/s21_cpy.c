#include "s21_string.h"

// void *memcpy(void *dest, const void *src, size_t n)
// void *memmove(void *dest, const void *src, size_t n)
// void *memset(void *str, int c, size_t n)
// char *strcpy(char *dest, const char *src)
// char *strncpy(char *dest, const char *src, size_t n)

void* s21_memcpy(void* restrict dest, const void* restrict src, size_t n) {
  return s21_memmove(dest, src, n);
}

void* s21_memmove(void* dest, const void* src, size_t n) {
  void* start_ptr = dest;

  char* temp_src = (char*)src;
  char* temp_dest = (char*)dest;

  for (size_t i = 0; i < n; ++i, ++temp_dest, ++temp_src) {
    *(temp_dest) = *(temp_src);
  }

  return start_ptr;
}

void* s21_memset(void* str, int c, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    ((char*)str)[i] = (char)c;
  }
  return str;
}

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