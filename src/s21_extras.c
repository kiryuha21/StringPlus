#include <ctype.h>
#include <stdlib.h>

#include "s21_string.h"

// void *s21_to_upper(const char *str);
// void *s21_to_lower(const char *str);
// void *s21_insert(const char *src, const char *str, size_t start_index);
// void *s21_trim(const char *src, const char *trim_chars);

void *change_case_common(const char *str, int to_upper) {
  size_t len = s21_strlen(str);

  char *result = (char *)calloc(sizeof(char), len + 1);
  if (result != NULL) {
    for (size_t i = 0; i < len; ++i) {
      result[i] = (char)(to_upper ? toupper(str[i]) : tolower(str[i]));
    }
  }
  return (void *)result;
}

void *s21_to_upper(const char *str) { return change_case_common(str, 1); }

void *s21_to_lower(const char *str) { return change_case_common(str, 0); }

void *s21_insert(const char *src, const char *str, size_t start_index) {
  size_t len = s21_strlen(str) + s21_strlen(str);

  char *result = (char *)calloc(sizeof(char), len + 1);
  if (result != NULL) {
    s21_strncat(result, src, start_index - 1);
    s21_strcat(result, str);
    s21_strcat(result, src + start_index);
  }

  return result;
}

void *s21_trim(const char *src, const char *trim_chars) {
  size_t len = s21_strlen(src);

  char *result = (char *)calloc(sizeof(char), len + 1);
  if (result != NULL) {
    size_t start = 0;
    for (; s21_strchr(trim_chars, src[start]) != NULL; ++start) {
    }

    size_t end = len - 1;
    for (; s21_strchr(trim_chars, src[end]) != NULL; --end) {
    }

    s21_strncpy(result, src + start, end - start);
  }

  return result;
}