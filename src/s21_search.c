#include "s21_string.h"

// void *memchr(const void *str, int c, s21_size_t n)
// char *strchr(const char *str, int c)
// char *strpbrk(const char *str1, const char *str2)
// char *strrchr(const char *str, int c)
// char *strstr(const char *haystack, const char *needle)

void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *res = S21_NULL;

  int stop = 0;
  const unsigned char *temp_str = (unsigned char *)str;
  for (s21_size_t i = 0; i < n && !stop; ++i) {
    if (temp_str[i] == c) {
      res = (void *)(&temp_str[i]);
      stop = 1;
    }
  }

  return res;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  for (const char *i = str1; *i != '\0'; ++i) {
    char *res = s21_strchr(str2, *i);
    if (res != S21_NULL) {
      return (char *)i;
    }
  }

  return S21_NULL;
}

char *s21_strrchr(const char *str, int c) {
  char *res = S21_NULL;

  int len = (int)s21_strlen(str);

  int found = 0;
  for (int i = len; i >= 0 && !found; --i) {
    if (str[i] == c) {
      res = (char *)&str[i];
      found = 1;
    }
  }

  return res;
}

char *s21_strchr(const char *src, int sym) {
  char *res = S21_NULL;
  int stop = 0;
  while (*src != '\0' && !stop) {
    if (*src == sym) {
      res = (char *)src;
      stop = 1;
    } else {
      ++src;
    }
  }
  if (!stop) {
    if (*src == sym) {
      res = (char *)src;
    }
  }
  return res;
}

int compare(const char *haystack, const char *needle) {
  int return_code = 1;
  int stop = 0;

  while (*haystack && *needle && !stop) {
    if (*haystack != *needle) {
      return_code = 0;
      stop = 1;
    }
    ++haystack;
    ++needle;
  }

  if (*needle != '\0') {
    return_code = 0;
  }
  return return_code;
}

char *s21_strstr(const char *haystack, const char *needle) {
  int stop = 0;
  char *result = S21_NULL;

  if (*needle == '\0') {
    return (char *)haystack;
  }

  while (*haystack != '\0' && !stop) {
    if ((*haystack == *needle) && compare(haystack, needle)) {
      result = (char *)haystack;
      stop = 1;
    }
    ++haystack;
  }

  return result;
}