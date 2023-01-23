#include <stdio.h>

#include "s21_string.h"

// char *strtok(char *str, const char *delim)
// char *strerror(int errnum)
// char *strcat(char *dest, const char *src)
// char *strncat(char *dest, const char *src, size_t n)

char* s21_strncat(char* dest, const char* src, size_t n) {
  size_t dest_len = s21_strlen(dest);
  return s21_strncpy(dest + dest_len, src, n) - dest_len;
}

char* s21_strcat(char* restrict dest, const char* restrict src) {
  return s21_strncat(dest, src, s21_strlen(src));
}

char* s21_strtok(char* restrict str, const char* restrict sep) {
  static char* last_stop;
  if (str == NULL) {
    str = last_stop;
  }
  char* res = str;
  if (str != NULL) {
    int code = 0;
    while (code == 0) {
      if (s21_strchr(sep, *str)) {
        ++str;
        ++res;
      } else if (*str == '\0') {
        code = 1;
      } else {
        code = 2;
      }
    }
    if (code != 1) {
      code = 0;
      while (code == 0) {
        if (*str == '\0') {
          last_stop = str;
          code = 1;
        } else if (s21_strchr(sep, *str)) {
          *str = '\0';
          last_stop = str + 1;
          code = 1;
        } else {
          ++str;
        }
      }
    } else {
      res = NULL;
    }
  } else {
    res = NULL;
  }
  return res;
}

char* s21_strerror(int errnum) {
  char* description = NULL;
  char default_description[20] = "Unknown error ";

  int found = 0;
  for (int i = 0; i < sys_nerr && !found; ++i) {
    if (sys_errlist[i].key == errnum) {
      found = 1;
      description = sys_errlist[i].value;
    }
  }

  if (!found) {
    sprintf(default_description + 14, "%d", errnum);
  }

  return found ? description : default_description;
}
