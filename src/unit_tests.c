#include <check.h>
#include <string.h>

#include "s21_string.h"

START_TEST(strlen_basic) {
  char *str = "normal string";
  size_t my_res = s21_strlen(str);
  size_t std_res = strlen(str);
  ck_assert_uint_eq(my_res, std_res);
}

START_TEST(strcat_basic) {
  char str1[15] = "Hello, ";
  char str2[15] = "Hello, ";

  strcat(str1, "World");
  s21_strcat(str2, "World");

  ck_assert_str_eq(str1, str2);
}

Suite *string_suite(void) {
  Suite *s = suite_create("String");

  TCase *strlen_cases = tcase_create("StrLen");
  tcase_add_test(strlen_cases, strlen_basic);

  TCase *strcat_cases = tcase_create("StrCat");
  tcase_add_test(strcat_cases, strcat_basic);

  suite_add_tcase(s, strlen_cases);
  suite_add_tcase(s, strcat_cases);

  return s;
}

int main(void) {
  Suite *s = string_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);

  srunner_free(sr);
  return 0;
}
