#include "s21_string.h"

// int memcmp(const void *str1, const void *str2, s21_size_t n)
// int strcmp(const char *str1, const char *str2)
// int strncmp(const char *str1, const char *str2, s21_size_t n)

int s21_memcmp(const void* str1, const void* str2, s21_size_t n) {
  unsigned char* temp1 = (unsigned char*)str1;
  unsigned char* temp2 = (unsigned char*)str2;

  for (s21_size_t i = 0; i < n; ++i) {
    if (temp1[i] != temp2[i]) {
      return temp1[i] - temp2[i];
    }
  }

  return 0;
}

int s21_strncmp(const char* str1, const char* str2, s21_size_t n) {
  for (s21_size_t i = 0; i < n; ++i) {
    if (str1[i] != str2[i]) {
      return (const unsigned char)str1[i] - (const unsigned char)str2[i];
    }
  }

  return 0;
}

int s21_strcmp(const char* str1, const char* str2) {
  s21_size_t first_len = s21_strlen(str1);
  s21_size_t second_len = s21_strlen(str2);

  return s21_strncmp(str1, str2,
                     first_len > second_len ? first_len : second_len);
}
