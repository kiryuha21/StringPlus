#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "s21_string.h"
#include "test_commons.h"

START_TEST(sprintf_ints) {
  int str_size = 10000;
  char* my_str = (char*)calloc(str_size, sizeof(char));
  char* std_str = (char*)calloc(str_size, sizeof(char));
  if (my_str != NULL && std_str != NULL) {
    int my_res, std_res;

    my_res = s21_sprintf(my_str, "%d   aaaa %lld %hd %hhd", 1000000000,
                         1000000000LL, 1000000000, 1000000000);
    std_res = sprintf(std_str, "%d   aaaa %lld %hd %hhd", 1000000000,
                      1000000000LL, 1000000000, 1000000000);
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);

    my_res = s21_sprintf(my_str, "%d   aaaa %lli %hi %hhi", 1000000000,
                         1000000000LL, 1000000000, 1000000000);
    std_res = sprintf(std_str, "%d   aaaa %lli %hi %hhi", 1000000000,
                      1000000000LL, 1000000000, 1000000000);
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);

    my_res = s21_sprintf(my_str, "%-#o", -10010);
    std_res = sprintf(std_str, "%-#o", -10010);
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);

    my_res = s21_sprintf(my_str, "%-#.o %-#.5o %-#10o%-#.1000o", -10010, -10010,
                         -10010, -10010);
    std_res = sprintf(std_str, "%-#.o %-#.5o %-#10o%-#.1000o", -10010, -10010,
                      -10010, -10010);
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);

    my_res = s21_sprintf(my_str, "%lc", L'\0');
    std_res = sprintf(std_str, "%lc", L'\0');
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);

    my_res = s21_sprintf(my_str, "%c %%", 'a');
    std_res = sprintf(std_str, "%c %%", 'a');
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);
  } else {
    printf("alloc error");
  }

  free(my_str);
  free(std_str);
}

START_TEST(sprintf_floats) {
  int str_size = 10000;
  char* my_str = (char*)calloc(str_size, sizeof(char));
  char* std_str = (char*)calloc(str_size, sizeof(char));
  if (my_str != NULL && std_str != NULL) {
    int my_res, std_res;

    my_res = s21_sprintf(my_str, "%f%e%g%2f%2e%2g%.2f%.2e%.2g", 1.4, 1.5, 1.6,
                         14.7, 13.8, 12.9, 2.32424, 3.2352, 2234.4);
    std_res = sprintf(std_str, "%f%e%g%2f%2e%2g%.2f%.2e%.2g", 1.4, 1.5, 1.6,
                      14.7, 13.8, 12.9, 2.32424, 3.2352, 2234.4);
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);

    my_res =
        s21_sprintf(my_str, "%Lf%Le%Lg%2Lf%2Le%2Lg%.2Lf%.2Le%.2Lg", 1.4L, 1.5L,
                    1.6L, 14.7L, 13.8L, 12.9L, 2.32424L, 3.2352L, 2234.4L);
    std_res =
        sprintf(std_str, "%Lf%Le%Lg%2Lf%2Le%2Lg%.2Lf%.2Le%.2Lg", 1.4L, 1.5L,
                1.6L, 14.7L, 13.8L, 12.9L, 2.32424L, 3.2352L, 2234.4L);
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);

    my_res = s21_sprintf(my_str, "%lf%le%lg%2lf%2le%2lg%.2lf%.2le%.2lg", 1.4,
                         1.5, 1.6, 14.7, 13.8, 12.9, 2.32424, 3.2352, 2234.4);
    std_res = sprintf(std_str, "%lf%le%lg%2lf%2le%2lg%.2lf%.2le%.2lg", 1.4, 1.5,
                      1.6, 14.7, 13.8, 12.9, 2.32424, 3.2352, 2234.4);
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);

    my_res = s21_sprintf(my_str, "%f %lf", NAN, INFINITY);
    std_res = sprintf(std_str, "%f %lf", NAN, INFINITY);
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);
  } else {
    printf("alloc error");
  }

  free(my_str);
  free(std_str);
}

START_TEST(sprintf_strings) {
  int str_size = 10000;
  char* my_str = (char*)calloc(str_size, sizeof(char));
  char* std_str = (char*)calloc(str_size, sizeof(char));
  if (my_str != NULL && std_str != NULL) {
    int my_res, std_res;

    my_res = s21_sprintf(my_str, "%10s", "Hello, world!");
    std_res = sprintf(std_str, "%10s", "Hello, world!");
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);

    my_res = s21_sprintf(my_str, "%10ls", L"Hello, world!");
    std_res = sprintf(std_str, "%10ls", L"Hello, world!");
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);
  } else {
    printf("alloc error");
  }

  free(my_str);
  free(std_str);
}

START_TEST(sprintf_pointers) {
  int str_size = 10000;
  char* my_str = (char*)calloc(str_size, sizeof(char));
  char* std_str = (char*)calloc(str_size, sizeof(char));
  if (my_str != NULL && std_str != NULL) {
    int my_res, std_res;

    my_res = s21_sprintf(my_str, "%20p", "Hello, world!");
    std_res = sprintf(std_str, "%20p", "Hello, world!");
    ck_assert_str_eq(my_str, std_str);
    ck_assert_int_eq(my_res, std_res);
    memset(my_str, '\0', str_size);
    memset(std_str, '\0', str_size);

  } else {
    printf("alloc error");
  }

  free(my_str);
  free(std_str);
}

START_TEST(sscanf_strings_chars) {
  char* b1 = calloc(20, sizeof(char));
  char* b2 = calloc(20, sizeof(char));
  char* b3 = calloc(20, sizeof(char));
  char* b4 = calloc(20, sizeof(char));

  if (b1 && b2 && b3 && b4) {
    int std_ret = sscanf("abc cdef", "%20s%3s", b1, b2);
    int my_ret = s21_sscanf("abc cdef", "%20s%3s", b3, b4);

    ck_assert_int_eq(std_ret, my_ret);
    ck_assert_str_eq(b1, b3);
    ck_assert_str_eq(b2, b4);
  }

  free(b1);
  free(b2);
  free(b3);
  free(b4);

  char a1, a2, a3, a4;
  int std_ret = sscanf(" a", "%c%c", &a1, &a2);
  int my_ret = s21_sscanf(" a", "%c%c", &a3, &a4);

  ck_assert_int_eq(std_ret, my_ret);
  ck_assert(a1 == a3);
  ck_assert(a2 == a4);
}

START_TEST(sscanf_floats) {
  float c1, c2;
  double d1, d2;
  long double e1, e2;

  int std_ret = sscanf("1.1e+1 1e+1 -1.2", "%f%lG%LG", &c1, &d1, &e1);
  int my_ret = s21_sscanf("1.1e+1 1e+1 -1.2", "%f%lG%LG", &c2, &d2, &e2);

  ck_assert_int_eq(std_ret, my_ret);
  ck_assert_float_eq_tol(c1, c2, 0.1);
  ck_assert_double_eq_tol(d1, d2, 0.1);
  ck_assert_ldouble_eq_tol(e1, e2, 0.1);
}

START_TEST(sscanf_signed) {
  signed char a1, a2;
  short b1, b2;
  int c1, c2;
  long d1, d2;
  long long e1, e2;

  int my_ret = s21_sscanf("100 32000 -2000000000 077777777777 -0xFFFFFFFFFF",
                          "%hhd%hd%i%li%lli", &a2, &b2, &c2, &d2, &e2);
  int std_ret = sscanf("100 32000 -2000000000 077777777777 -0xFFFFFFFFFF",
                       "%hhd%hd%i%li%lli", &a1, &b1, &c1, &d1, &e1);

  ck_assert_int_eq(std_ret, my_ret);

  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
  ck_assert_int_eq(c1, c2);
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(e1, e2);
}

START_TEST(sscanf_unsigned) {
  unsigned char a1, a2;
  unsigned short b1, b2;
  unsigned int c1, c2;
  unsigned long d1, d2;
  unsigned long long e1, e2;

  int my_ret = s21_sscanf("100 32000 0xFFFFFF 077777777777 0xffffffffff",
                          "%hhu%hu%x%lo%llX", &a2, &b2, &c2, &d2, &e2);
  int std_ret = sscanf("100 32000 0xFFFFFF 077777777777 0xffffffffff",
                       "%hhu%hu%x%lo%llX", &a1, &b1, &c1, &d1, &e1);

  ck_assert_int_eq(std_ret, my_ret);

  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
  ck_assert_int_eq(c1, c2);
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(e1, e2);
}

START_TEST(sscanf_skip_assignment) {
  int a1, a2;

  int my_res = s21_sscanf("123 345", "%*d%d", &a2);
  int std_res = sscanf("123 345", "%*d%d", &a1);

  ck_assert_int_eq(std_res, my_res);
  ck_assert_int_eq(a1, a2);
}

START_TEST(sscanf_extra_values) {
  double a1, a2;

  int std_res = sscanf("NAN", "%lf", &a1);
  int my_res = s21_sscanf("NAN", "%lf", &a2);

  ck_assert_int_eq(std_res, my_res);
  ck_assert(isnan(a1));
  ck_assert(isnan(a2));

  std_res = sscanf("INF", "%lf", &a1);
  my_res = s21_sscanf("INF", "%lf", &a2);

  ck_assert_int_eq(std_res, my_res);
  ck_assert(isinf(a1));
  ck_assert(isinf(a2));
}

START_TEST(sscanf_wchar_values) {
  wchar_t* a1 = calloc(20, sizeof(wchar_t));
  wchar_t* a2 = calloc(20, sizeof(wchar_t));

  int std_res = sscanf("abcdef", "%ls", a1);
  int my_res = s21_sscanf("abcdef", "%ls", a2);

  ck_assert_int_eq(std_res, my_res);
  ck_assert(wcscmp(a1, a2) == 0);

  wchar_t b1, b2;

  std_res = sscanf(" a", "%lc", &b1);
  my_res = s21_sscanf(" a", "%lc", &b2);

  ck_assert_int_eq(std_res, my_res);
  ck_assert(b1 == b2);
}

START_TEST(sscanf_pointer) {
  void *p1, *p2, *p3, *p4;

  int std_res = sscanf("(nil) 0xFFFFF", "%p%p", &p1, &p2);
  int my_res = s21_sscanf("(nil) 0xFFFFF", "%p%p", &p3, &p4);

  ck_assert_int_eq(std_res, my_res);
  ck_assert_ptr_eq(p1, p3);
  ck_assert_ptr_eq(p2, p4);
}

START_TEST(sscanf_errors) {
  int a1, a2;

  int std_ret = sscanf("", "%n", &a1);
  int my_ret = s21_sscanf("", "%n", &a2);

  ck_assert_int_eq(std_ret, my_ret);
  ck_assert_int_eq(a1, a2);

  int a3, a4;

  std_ret = sscanf("", "%n%d", &a1, &a3);
  my_ret = s21_sscanf("", "%n%d", &a2, &a4);

  ck_assert_int_eq(std_ret, my_ret);
}

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

  TCase* sprintf_cases = tcase_create("SPrintF");
  tcase_add_test(sprintf_cases, sprintf_ints);
  tcase_add_test(sprintf_cases, sprintf_floats);
  tcase_add_test(sprintf_cases, sprintf_strings);
  tcase_add_test(sprintf_cases, sprintf_pointers);

  TCase* sscanf_cases = tcase_create("SSCanF");
  tcase_add_test(sscanf_cases, sscanf_strings_chars);
  tcase_add_test(sscanf_cases, sscanf_floats);
  tcase_add_test(sscanf_cases, sscanf_errors);
  tcase_add_test(sscanf_cases, sscanf_signed);
  tcase_add_test(sscanf_cases, sscanf_unsigned);
  tcase_add_test(sscanf_cases, sscanf_extra_values);
  tcase_add_test(sscanf_cases, sscanf_pointer);
  tcase_add_test(sscanf_cases, sscanf_wchar_values);
  tcase_add_test(sscanf_cases, sscanf_skip_assignment);

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
  suite_add_tcase(s, sprintf_cases);
  suite_add_tcase(s, sscanf_cases);

  return s;
}

int main(void) {
  Suite* s = string_suite();
  SRunner* sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);

  srunner_free(sr);
  return 0;
}
