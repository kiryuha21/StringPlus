#include <check.h>
#include <stdio.h>
#include <string.h>

#include "s21_format_functions.h"
#include "s21_string.h"
#include "test_commons.h"

START_TEST(strlen_basic) { strlen_test_common("normal string"); }

START_TEST(strlen_empty) { strlen_test_common(""); }

START_TEST(strlen_with_null_terminator) { strlen_test_common("aaa\0bbb"); }

START_TEST(memcmp_equal) { memcmp_test_common("aaa\0bbb\n", "aaa\0bbb\n", 7); }

START_TEST(memcmp_unequal) {
  memcmp_test_common("aaa\0ccc\n", "aaa\0bbb\n", 7);
}

START_TEST(memcmp_size_limit) {
  memcmp_test_common("aaa\0bbb\n", "aaa\0ccc\n", 4);
}

START_TEST(memcmp_overflow_limit) {
  memcmp_test_common("aaa\0bbb\n", "aaa\0bbb\n", 20);
}

START_TEST(strerror_basic) {
  for (int i = 0; i < sys_nerr + 1; ++i) {
    ck_assert_str_eq(strerror(i), s21_strerror(i));
  }
}

START_TEST(strrchr_basic) {
  char* str = "normal string";
  char* my_res = s21_strrchr(str, 'n');
  char* std_res = strrchr(str, 'n');
  ck_assert_str_eq(my_res, std_res);
}

START_TEST(strncat_basic) {
  char str1[15] = "Hello, ";
  char str2[15] = "Hello, ";

  char* std_res = strncat(str1, "Worlds", 5);
  char* my_res = s21_strncat(str2, "Worlds", 5);

  ck_assert_str_eq(std_res, my_res);
}

START_TEST(strcat_basic) {
  char str1[15] = "Hello, ";
  char str2[15] = "Hello, ";

  char* std_res = strcat(str1, "World");
  char* my_res = s21_strcat(str2, "World");

  ck_assert_str_eq(std_res, my_res);
}

START_TEST(strcpy_basic) {
  char str1[30] = "Hello, ";
  char str2[30] = "Hello, ";
  char* value = "Good Evening\0, Everyone";

  char* std_res = strcpy(str1, value);
  char* my_res = s21_strcpy(str2, value);

  ck_assert_str_eq(std_res, my_res);
}

START_TEST(memchr_basic) {
  char* haystack = "aaa\0bcd\n";

  char* std_res = memchr(haystack, 'c', 8);
  char* my_res = s21_memchr(haystack, 'c', 8);

  ck_assert_mem_eq(std_res, my_res, 3);
}

START_TEST(strpbrk_basic) {
  char* haystack = "aaabbbcde";
  char* entries = "zxc";

  char* std_res = strpbrk(haystack, entries);
  char* my_res = s21_strpbrk(haystack, entries);

  ck_assert_str_eq(std_res, my_res);
}

START_TEST(memcpy_basic) {
  char str1[256];
  char str2[256];
  char value[256];

  for (int i = 0; i < 256; ++i) {
    sprintf(value + i, "%c", i);
  }

  char* std_res = memcpy(str1, value, 256);
  char* my_res = s21_memcpy(str2, value, 256);

  ck_assert_mem_eq(std_res, my_res, 256);
}

START_TEST(memset_basic) {
  char str1[15] = "Hello, ";
  char str2[15] = "Hello, ";

  char* std_res = memset(str1, 'a', 4);
  char* my_res = s21_memset(str2, 'a', 4);

  ck_assert_str_eq(std_res, my_res);
}

START_TEST(to_upper_basic) {
  change_case_test_common("aAa 111 BbB", "AAA 111 BBB", 1);
}

START_TEST(to_lower_basic) {
  change_case_test_common("aAa 111 BbB", "aaa 111 bbb", 0);
}

START_TEST(insert_basic) {
  insert_test_common("Hello, World!", "Amazing ", 7, "Hello, Amazing World!");
}

START_TEST(trim_basic) {
  trim_test_common("\n\tHello\nto\teveryone\n\t", "\n\t",
                   "Hello\nto\teveryone");
}

START_TEST(strcmp_basic) {
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

  strtok_test_common(s21_haystack, str_haystack, ",-");
}

START_TEST(strtok_doesnt_find) {
  char* s21_haystack = "Hello, world!";
  char* str_haystack = "Hello, world!";

  strtok_test_common(s21_haystack, str_haystack, ".");
}

START_TEST(strtok_only_delims) {
  char* s21_haystack = "._.";
  char* str_haystack = "._.";

  strtok_test_common(s21_haystack, str_haystack, "._");
}

START_TEST(strcspn_basic) { strspn_test_common("000111222", "21", 0); }

START_TEST(strcspn_zero_res) { strspn_test_common("000111222", "0", 0); }

START_TEST(strspn_basic) { strspn_test_common("000111222", "01", 1); }

START_TEST(strspn_zero_res) { strspn_test_common("000111222", "12", 1); }

// TODO: Add more tests lol
Suite* string_suite(void) {
  Suite* s = suite_create("String");

  TCase* strlen_cases = tcase_create("StrLen");
  tcase_add_test(strlen_cases, strlen_basic);
  tcase_add_test(strlen_cases, strlen_empty);
  tcase_add_test(strlen_cases, strlen_with_null_terminator);

  TCase* memcmp_cases = tcase_create("MemCmp");
  tcase_add_test(memcmp_cases, memcmp_equal);
  tcase_add_test(memcmp_cases, memcmp_unequal);
  tcase_add_test(memcmp_cases, memcmp_size_limit);
  tcase_add_test(memcmp_cases, memcmp_overflow_limit);

  TCase* strerror_cases = tcase_create("StrError");
  tcase_add_test(strerror_cases, strerror_basic);

  TCase* memchr_cases = tcase_create("MemChr");
  tcase_add_test(memchr_cases, memchr_basic);

  TCase* strpbrk_cases = tcase_create("StrPBrk");
  tcase_add_test(strpbrk_cases, strpbrk_basic);

  TCase* to_lower_cases = tcase_create("ToLower");
  tcase_add_test(to_lower_cases, to_lower_basic);

  TCase* to_upper_cases = tcase_create("ToUpper");
  tcase_add_test(to_upper_cases, to_upper_basic);

  TCase* insert_cases = tcase_create("Insert");
  tcase_add_test(insert_cases, insert_basic);

  TCase* trim_cases = tcase_create("Trim");
  tcase_add_test(trim_cases, trim_basic);

  TCase* strcat_cases = tcase_create("StrCat");
  tcase_add_test(strcat_cases, strcat_basic);

  TCase* strncat_cases = tcase_create("StrNCat");
  tcase_add_test(strncat_cases, strncat_basic);

  TCase* strcpy_cases = tcase_create("StrCpy");
  tcase_add_test(strcpy_cases, strcpy_basic);

  TCase* strcmp_cases = tcase_create("StrCmp");
  tcase_add_test(strcmp_cases, strcmp_basic);

  TCase* strchr_cases = tcase_create("StrChr");
  tcase_add_test(strchr_cases, strchr_basic);

  TCase* strstr_cases = tcase_create("StrStr");
  tcase_add_test(strstr_cases, strstr_basic);
  tcase_add_test(strstr_cases, strstr_doesnt_find);

  TCase* strtok_cases = tcase_create("StrTok");
  tcase_add_test(strtok_cases, strtok_basic);
  tcase_add_test(strtok_cases, strtok_doesnt_find);
  tcase_add_test(strtok_cases, strtok_only_delims);

  TCase* memcpy_cases = tcase_create("MemCpy");
  tcase_add_test(memcpy_cases, memcpy_basic);

  TCase* memset_cases = tcase_create("MemSet");
  tcase_add_test(memset_cases, memset_basic);

  TCase* strrchr_cases = tcase_create("StrRChr");
  tcase_add_test(strrchr_cases, strrchr_basic);

  TCase* strspn_cases = tcase_create("StrSpn");
  tcase_add_test(strspn_cases, strspn_basic);
  tcase_add_test(strspn_cases, strspn_zero_res);

  TCase* strcspn_cases = tcase_create("StrCSpn");
  tcase_add_test(strcspn_cases, strcspn_basic);
  tcase_add_test(strcspn_cases, strcspn_zero_res);

  suite_add_tcase(s, strlen_cases);
  suite_add_tcase(s, strerror_cases);
  suite_add_tcase(s, strcat_cases);
  suite_add_tcase(s, strncat_cases);
  suite_add_tcase(s, strcmp_cases);
  suite_add_tcase(s, strchr_cases);
  suite_add_tcase(s, strstr_cases);
  suite_add_tcase(s, strtok_cases);
  suite_add_tcase(s, memcpy_cases);
  suite_add_tcase(s, memset_cases);
  suite_add_tcase(s, strrchr_cases);
  suite_add_tcase(s, strspn_cases);
  suite_add_tcase(s, strcspn_cases);
  suite_add_tcase(s, memcmp_cases);
  suite_add_tcase(s, memchr_cases);
  suite_add_tcase(s, strpbrk_cases);
  suite_add_tcase(s, strcpy_cases);
  suite_add_tcase(s, to_lower_cases);
  suite_add_tcase(s, to_upper_cases);
  suite_add_tcase(s, insert_cases);
  suite_add_tcase(s, trim_cases);

  return s;
}

// TODO: remove
#include <stdlib.h>

int main(void) {
  Suite* s = string_suite();
  SRunner* sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);

  srunner_free(sr);

  // TODO: remove
  char* a = (char*)calloc(sizeof(char), 100);
  s21_sprintf(a, "%20d", 12);
  printf("%s\n", a);
  s21_sprintf(a, "%+4d", -12);
  printf("%s\n", a);
  s21_sprintf(a, "%4d", 12);
  printf("%s\n", a);

  s21_sprintf(a, "%4d", -12);
  printf("%s\n", a);

  return 0;
}
