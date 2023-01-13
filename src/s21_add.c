#include "s21_string.h"

// char *strtok(char *str, const char *delim)
// char *strerror(int errnum)
// char *strcat(char *dest, const char *src)
// char *strncat(char *dest, const char *src, size_t n)

char* s21_strcat(char* restrict destination, const char* restrict source) {
  size_t dst_len = s21_strlen(destination);
  return s21_strcpy(destination + dst_len, source) - dst_len;
}

char* s21_strtok(char* restrict str, const char* delim) {
  static char* backup;
  char* return_value = NULL;

  if (str == NULL) {
    str = backup;
  }
  if (str != NULL) {
    int stop = 0;
    while (s21_strchr(delim, *str) && !stop) {
      ++str;
      if (*str == '\0') {
        stop = 1;
      }
    }
    if (!stop) {
      return_value = str;
      while (!stop) {
        if (*str == '\0') {
          backup = str;
          stop = 1;
        } else if (s21_strchr(delim, *str)) {
          *str = '\0';
          backup = str + 1;
          stop = 1;
        } else {
          ++str;
        }
      }
    }
  }

  return return_value;
}