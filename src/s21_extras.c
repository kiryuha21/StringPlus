#include <ctype.h>
#include <stdlib.h>

#include "s21_string.h"

// void *s21_to_upper(const char *str);
// void *s21_to_lower(const char *str);
// void *s21_insert(const char *src, const char *str, s21_size_t start_index);
// void *s21_trim(const char *src, const char *trim_chars);

void *change_case_common(const char *str, int to_upper) {
  s21_size_t len = s21_strlen(str);
  void *ptr = S21_NULL;
  if (str != S21_NULL) {
    ptr = calloc(len + 1, sizeof(char));
    if (ptr != S21_NULL) {
      for (s21_size_t i = 0; i < len; ++i) {
        ((char *)ptr)[i] = (char)(to_upper ? toupper(str[i]) : tolower(str[i]));
      }
    }
  }
  return ptr;
}

void *s21_to_upper(const char *str) { return change_case_common(str, 1); }

void *s21_to_lower(const char *str) { return change_case_common(str, 0); }

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  s21_size_t len = s21_strlen(src) + s21_strlen(str);

  if (src == S21_NULL || str == S21_NULL || start_index > s21_strlen(src)) {
    return S21_NULL;
  }

  char *result = (char *)calloc(len + 1, sizeof(char));
  if (result != S21_NULL) {
    s21_strncat(result, src, start_index);
    s21_strcat(result, str);
    s21_strcat(result, src + start_index);
  }

  return result;
}

void *s21_trim(const char *src, const char *trim_chars) {
  s21_size_t len = s21_strlen(src);

  if (src == S21_NULL) {
    return S21_NULL;
  }

  char *result = (char *)calloc(len + 1, sizeof(char));

  if (result == S21_NULL) {
    return S21_NULL;
  }

  s21_size_t start = 0;
  s21_size_t end = len - 1;

  if (trim_chars != S21_NULL) {
    for (; strchr_without_terminator(trim_chars, src[end]) != 0; --end) {
    }
    for (; strchr_without_terminator(trim_chars, src[start]) != 0; ++start) {
    }
  }

  s21_strncpy(result, src + start, end - start + 1);

  return result;
}