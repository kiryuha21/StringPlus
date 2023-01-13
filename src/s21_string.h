#ifndef C2_S21_STRINGPLUS_0_S21_STRING_H
#define C2_S21_STRINGPLUS_0_S21_STRING_H

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned long size_t;

// s21_search.c:
// void *memchr(const void *str, int c, size_t n)
char* s21_strchr(const char* src, int sym);
// char *strpbrk(const char *str1, const char *str2)
// char *strrchr(const char *str, int c)
char* s21_strstr(const char* haystack, const char* needle);

// s21_add.c:
char* s21_strtok(char* restrict str, const char* restrict delim);
// char *strerror(int errnum)
char* s21_strcat(char* restrict destination, const char* restrict source);
// char *strncat(char *dest, const char *src, size_t n)

// s21_cpy.c:
// void *memcpy(void *dest, const void *src, size_t n)
// void *memmove(void *dest, const void *src, size_t n)
// void *memset(void *str, int c, size_t n)
char* s21_strcpy(char* restrict destination, const char* restrict source);
// char *strncpy(char *dest, const char *src, size_t n)

// s21_cmp.c:
// int memcmp(const void *str1, const void *str2, size_t n)
int s21_strcmp(const char* first, const char* second);
// int strncmp(const char *str1, const char *str2, size_t n)

// s21_calc.c:
size_t s21_strlen(const char* str);
// size_t strspn(const char *str1, const char *str2)
// size_t strcspn(const char *str1, const char *str2)

#endif  // C2_S21_STRINGPLUS_0_S21_STRING_H
