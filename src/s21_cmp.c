#include "s21_string.h"

// int memcmp(const void *str1, const void *str2, size_t n)
// int strcmp(const char *str1, const char *str2)
// int strncmp(const char *str1, const char *str2, size_t n)

int s21_memcmp(const void* str1, const void* str2, size_t n) {
  unsigned char* temp1 = (unsigned char*)str1;
  unsigned char* temp2 = (unsigned char*)str2;

  for (size_t i = 0; *temp1 == *temp2 && i < n; ++i, ++temp1, ++temp2) {
  }

  return *temp1 - *temp2;
}

int s21_strncmp(const char* str1, const char* str2, size_t n) {
  for (size_t i = 0; *str1 != '\0' && *str1 == *str2 && i < n; ++i) {
    ++str1;
    ++str2;
  }

  return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

int s21_strcmp(const char* str1, const char* str2) {
  size_t first_len = s21_strlen(str1);
  size_t second_len = s21_strlen(str2);

  return s21_strncmp(str1, str2,
                     first_len > second_len ? first_len : second_len);
}
