#include "s21_string.h"

// s21_size_t strlen(const char *str)
// s21_size_t strspn(const char *str1, const char *str2)
// s21_size_t strcspn(const char *str1, const char *str2)

s21_size_t s21_strlen(const char *str) {
  s21_size_t count = 0;
  if (str != S21_NULL) {
    for (const char *sym = str; *sym != '\0'; ++sym, ++count) {
    }
  }
  return count;
}

s21_size_t common_strspn(const char *haystack, const char *entries,
                         int should_be_in) {
  s21_size_t result = 0;

  s21_size_t haystack_len = s21_strlen(haystack);

  int stop = 0;
  for (s21_size_t i = 0; i < haystack_len && !stop; ++i) {
    if (s21_strchr(entries, haystack[i]) != S21_NULL) {
      if (should_be_in) {
        ++result;
      } else {
        stop = 1;
      }
    } else {
      if (should_be_in) {
        stop = 1;
      } else {
        ++result;
      }
    }
  }

  return result;
}

s21_size_t s21_strspn(const char *haystack, const char *entries) {
  return common_strspn(haystack, entries, 1);
}

s21_size_t s21_strcspn(const char *haystack, const char *entries) {
  return common_strspn(haystack, entries, 0);
}
