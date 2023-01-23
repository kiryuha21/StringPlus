#include "s21_string.h"

// size_t strlen(const char *str)
// size_t strspn(const char *str1, const char *str2)
// size_t strcspn(const char *str1, const char *str2)

size_t s21_strlen(const char *str) {
  size_t count = 0;
  if (str != NULL) {
    for (const char *sym = str; *sym != '\0'; ++sym, ++count) {
    }
  }
  return count;
}

size_t common_strspn(const char *haystack, const char *entries,
                     int should_be_in) {
  size_t result = 0;

  size_t haystack_len = s21_strlen(haystack);

  int stop = 0;
  for (size_t i = 0; i < haystack_len && !stop; ++i) {
    if (s21_strchr(entries, haystack[i]) != NULL) {
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

size_t s21_strspn(const char *haystack, const char *entries) {
  return common_strspn(haystack, entries, 1);
}

size_t s21_strcspn(const char *haystack, const char *entries) {
  return common_strspn(haystack, entries, 0);
}
