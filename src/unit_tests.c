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

START_TEST(strcmp_bacis) {
    char* first = "test";
    char* second = "test";

    int my_res = s21_strcmp(first, second);
    int std_res = strcmp(first, second);

    ck_assert_int_eq(my_res, std_res);
}

START_TEST(strchr_basic) {
    char* haystack = "tests";

    char* my_res = s21_strchr(haystack, 's');
    char* std_res = strchr(haystack, 's');

    ck_assert_str_eq(my_res, std_res);
}

START_TEST(strstr_basic) {
    char* haystack = "test";

    char* my_res = s21_strstr(haystack, "st");
    char* std_res = strstr(haystack, "st");

    ck_assert_str_eq(my_res, std_res);
}

START_TEST(strstr_doesnt_find) {
    char* haystack = "test";

    char* my_res = s21_strstr(haystack, "not_exist");
    char* std_res = strstr(haystack, "not_exist");

    ck_assert_ptr_eq(my_res, std_res);
}

START_TEST(strtok_basic) {
    char s21_haystack[] = "co\no,l un--it tee\n\n,est";
    char str_haystack[] = "co\no,l un--it tee\n\n,est";

    char* my_res = s21_strtok(s21_haystack, ",-");
    char* std_res = strtok(str_haystack, ",-");

    while (my_res && std_res) {
        ck_assert_str_eq(my_res, std_res);
        my_res = s21_strtok(NULL, ",-");
        std_res = strtok(NULL, ",-");
    }

    ck_assert_str_eq(s21_haystack, str_haystack);
    ck_assert_ptr_eq(my_res, std_res);
}

Suite *string_suite(void) {
  Suite *s = suite_create("String");

  TCase *strlen_cases = tcase_create("StrLen");
  tcase_add_test(strlen_cases, strlen_basic);

  TCase *strcat_cases = tcase_create("StrCat");
  tcase_add_test(strcat_cases, strcat_basic);

  TCase *strcmp_cases = tcase_create("StrCmp");
  tcase_add_test(strcmp_cases, strcmp_bacis);

  TCase *strchr_cases = tcase_create("StrChr");
  tcase_add_test(strchr_cases, strchr_basic);

  TCase *strstr_cases = tcase_create("StrStr");
  tcase_add_test(strstr_cases, strstr_basic);
  tcase_add_test(strstr_cases, strstr_doesnt_find);

  TCase *strtok_cases = tcase_create("StrTok");
  tcase_add_test(strtok_cases, strtok_basic);

  suite_add_tcase(s, strlen_cases);
  suite_add_tcase(s, strcat_cases);
  suite_add_tcase(s, strcmp_cases);
  suite_add_tcase(s, strchr_cases);
  suite_add_tcase(s, strstr_cases);
  suite_add_tcase(s, strtok_cases);

  return s;
}

int main(void) {
  Suite *s = string_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);

  srunner_free(sr);
  return 0;
}
