#include "s21_string.h"

// size_t strlen(const char *str)
// size_t strspn(const char *str1, const char *str2)
// size_t strcspn(const char *str1, const char *str2)

size_t s21_strlen(const char* str) {
  size_t count = 0;
  if (str != NULL) {
    for (const char* sym = str; *sym != '\0'; ++sym, ++count) {
    }
  }
  return count;
}
