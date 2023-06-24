#include "test_commons.h"

#include <check.h>
#include <stdlib.h>
#include <string.h>

void strtok_test_common(char s21_haystack[], char str_haystack[],
                        char delim[]) {
  char *my_res = s21_strtok(s21_haystack, delim);
  char *std_res = strtok(str_haystack, delim);

  while (my_res && std_res) {
    ck_assert_str_eq(my_res, std_res);
    my_res = s21_strtok(NULL, delim);
    std_res = strtok(NULL, delim);
  }

  ck_assert_str_eq(s21_haystack, str_haystack);
  ck_assert_ptr_eq(my_res, std_res);
}

void strlen_test_common(char *str) {
  s21_size_t my_res = s21_strlen(str);
  s21_size_t std_res = strlen(str);
  ck_assert_uint_eq(my_res, std_res);
}

void memcmp_test_common(char *first, char *second, s21_size_t len) {
  ck_assert_uint_eq(memcmp(first, second, len), s21_memcmp(first, second, len));
}

void strspn_test_common(char *haystack, char *entries, int should_be_in) {
  if (should_be_in) {
    ck_assert_uint_eq(strspn(haystack, entries), s21_strspn(haystack, entries));
  } else {
    ck_assert_uint_eq(strcspn(haystack, entries),
                      s21_strcspn(haystack, entries));
  }
}

void change_case_test_common(const char *origin, const char *expected,
                             int to_upper) {
  char *my_res = to_upper ? s21_to_upper(origin) : s21_to_lower(origin);

  ck_assert_str_eq(my_res, expected);

  free(my_res);
}

void insert_test_common(const char *origin, const char *insert_value, int pos,
                        const char *expected) {
  char *my_res = s21_insert(origin, insert_value, pos);

  ck_assert_str_eq(my_res, expected);

  free(my_res);
}

void trim_test_common(const char *origin, const char *trim_chars,
                      const char *expected) {
  char *my_res = s21_trim(origin, trim_chars);

  ck_assert_str_eq(my_res, expected);

  free(my_res);
}
