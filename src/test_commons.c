#include "test_commons.h"

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_debug(char* format, void* values, Types type, char* my_res,
                 char* std_res, int full) {
  if (full) {
    printf("---------\nformat:\n%s\n", format);
    if (type == INT) {
      printf("val:\n%d\n", *((int*)values));
    } else if (type == STRING) {
      printf("val:\n%s\n", (char*)values);
    } else if (type == CHAR) {
      printf("val:\n%c -(%d)\n", *((char*)values), (int)*((char*)values));
    } else if (type == DOUBLE) {
      printf("val:\n%f\n", *((double*)values));
    }
    printf("results(my - std):\n\"%s\"\n\"%s\"\n---------\n", my_res, std_res);
  } else {
    printf("%s | ", format);
    if (type == INT) {
      printf("%d", *((int*)values));
    } else if (type == STRING) {
      printf("%s", (char*)values);
    } else if (type == CHAR) {
      printf("%c", *((char*)values));
    } else if (type == DOUBLE) {
      printf("%f", *((double*)values));
    }
    printf(" | \"%s\" | \"%s\"\n", my_res, std_res);
  }
}

int sprintf_test_common(char* format, void* values, Types type, int assert) {
  // TODO: do smth about max len
  char my_res[10000], std_res[10000];
  int my_ret = 0, std_ret = 1;

  s21_memset(my_res, '\0', 10000);

  if (type == INT) {
    std_ret = sprintf(std_res, format, *((int*)values));
    my_ret = s21_sprintf(my_res, format, *((int*)values));
  } else if (type == STRING) {
    std_ret = sprintf(std_res, format, (char*)values);
    my_ret = s21_sprintf(my_res, format, (char*)values);
  } else if (type == CHAR) {
    std_ret = sprintf(std_res, format, *((char*)values));
    my_ret = s21_sprintf(my_res, format, *((char*)values));
  } else if (type == DOUBLE) {
    std_ret = sprintf(std_res, format, *((double*)values));
    my_ret = s21_sprintf(my_res, format, *((double*)values));
  }

  if (s21_strcmp(my_res, std_res) || my_ret != std_ret) {
    print_debug(format, values, type, my_res, std_res, 1);
  }
#ifdef DEBUG
  else {
    print_debug(format, values, type, my_res, std_res, 0);
  }
#endif
  if (assert) {
    ck_assert_str_eq(my_res, std_res);
  }
  return (s21_strcmp(my_res, std_res) || my_ret != std_ret) ? 1 : 0;
}

void strtok_test_common(char s21_haystack[], char str_haystack[],
                        char delim[]) {
  char* my_res = s21_strtok(s21_haystack, delim);
  char* std_res = strtok(str_haystack, delim);

  while (my_res && std_res) {
    ck_assert_str_eq(my_res, std_res);
    my_res = s21_strtok(NULL, delim);
    std_res = strtok(NULL, delim);
  }

  ck_assert_str_eq(s21_haystack, str_haystack);
  ck_assert_ptr_eq(my_res, std_res);
}

void strlen_test_common(char* str) {
  size_t my_res = s21_strlen(str);
  size_t std_res = strlen(str);
  ck_assert_uint_eq(my_res, std_res);
}

void memcmp_test_common(char* first, char* second, size_t len) {
  ck_assert_uint_eq(memcmp(first, second, len), s21_memcmp(first, second, len));
}

void strspn_test_common(char* haystack, char* entries, int should_be_in) {
  if (should_be_in) {
    ck_assert_uint_eq(strspn(haystack, entries), s21_strspn(haystack, entries));
  } else {
    ck_assert_uint_eq(strcspn(haystack, entries),
                      s21_strcspn(haystack, entries));
  }
}

void change_case_test_common(const char* origin, const char* expected,
                             int to_upper) {
  char* my_res = to_upper ? s21_to_upper(origin) : s21_to_lower(origin);

  ck_assert_str_eq(my_res, expected);

  free(my_res);
}

void insert_test_common(const char* origin, const char* insert_value, int pos,
                        const char* expected) {
  char* my_res = s21_insert(origin, insert_value, pos);

  ck_assert_str_eq(my_res, expected);

  free(my_res);
}

void trim_test_common(const char* origin, const char* trim_chars,
                      const char* expected) {
  char* my_res = s21_trim(origin, trim_chars);

  ck_assert_str_eq(my_res, expected);

  free(my_res);
}
