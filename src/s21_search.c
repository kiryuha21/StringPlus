#include "s21_string.h"

// void *memchr(const void *str, int c, size_t n)
// char *strchr(const char *str, int c)
// char *strpbrk(const char *str1, const char *str2)
// char *strrchr(const char *str, int c)
// char *strstr(const char *haystack, const char *needle)

void* s21_memchr(const void* str, int c, size_t n) {
  void* res = NULL;

  int stop = 0;
  const unsigned char* temp_str = (unsigned char*)str;
  for (size_t i = 0; i < n && !stop; ++i) {
    if (temp_str[i] == c) {
      res = (void*)(&temp_str[i]);
      stop = 1;
    }
  }

  return res;
}

char* s21_strpbrk(const char* str1, const char* str2) {
  char* res = NULL;

  int found = 0;
  for (const char* i = str2; *i != '\0' && !found; ++i) {
    res = s21_strchr(str1, *i);
    if (res != NULL) {
      found = 1;
    }
  }

  return res;
}

char* s21_strrchr(const char* str, int c) {
  char* res = NULL;

  int len = (int)s21_strlen(str);

  int found = 0;
  for (int i = len - 1; i >= 0 && !found; --i) {
    if (str[i] == c) {
      res = (char*)&str[i];
      found = 1;
    }
  }

  return res;
}

char* s21_strchr(const char* src, int sym) {
    char* res = NULL;
    int stop = 0;
    while (*src != '\0' && !stop) {
        if (*src == sym) {
            res = (char*)src;
            stop = 1;
        } else {
            ++src;
        }
    }
    return res;
}

int compare(const char* haystack, const char* needle) {
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

char* s21_strstr(const char* haystack, const char* needle) {
  int stop = 0;
  char* result = NULL;

  while (*haystack != '\0' && !stop) {
    if ((*haystack == *needle) && compare(haystack, needle)) {
      result = (char*)haystack;
      stop = 1;
    }
    ++haystack;
  }

  return result;
}