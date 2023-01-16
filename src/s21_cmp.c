#include "s21_string.h"

// int memcmp(const void *str1, const void *str2, size_t n)
// int strcmp(const char *str1, const char *str2)
// int s21_strncmp(const char *str1, const char *str2, size_t n)

// TODO: Not sure to which type should void* memory be casted;
// TODO: a lot of funcs depend on that, all of them should be rechecked in case
// TODO: of change
int s21_memcmp(const void* str1, const void* str2, size_t n) {
  char* temp1 = (char*)str1;
  char* temp2 = (char*)str2;

  for (size_t i = 0; *temp1 == *temp2 && i < n; ++i, ++temp1, ++temp2) {
  }

  return *(const unsigned char*)temp1 - *(const unsigned char*)temp2;
}

int s21_strncmp(const char* str1, const char* str2, size_t n) {
  size_t first_len = s21_strlen(str1);
  size_t second_len = s21_strlen(str2);
  size_t biggest_len = first_len > second_len ? first_len : second_len;
  size_t smallest_len = first_len > second_len ? second_len : first_len;
  size_t required_len = n < smallest_len ? n : biggest_len;

  return s21_memcmp((void*)str1, (void*)str2, required_len);
}

int s21_strcmp(const char* first, const char* second) {
  while (*first != '\0' && *first == *second) {
    ++first;
    ++second;
  }

  return *(const unsigned char*)first - *(const unsigned char*)second;
}