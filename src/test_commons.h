#ifndef C2_S21_STRINGPLUS_0_TEST_COMMONS_H
#define C2_S21_STRINGPLUS_0_TEST_COMMONS_H

#include "s21_sprintf.h"
#include "s21_sscanf.h"
#include "s21_string.h"

void strtok_test_common(char s21_haystack[], char str_haystack[], char delim[]);
void strlen_test_common(char* str);
void strspn_test_common(char* haystack, char* entries, int should_be_in);
void memcmp_test_common(char* first, char* second, size_t len);
void change_case_test_common(const char* origin, const char* expected,
                             int to_upper);
void insert_test_common(const char* origin, const char* insert_value, int pos,
                        const char* expected);
void trim_test_common(const char* origin, const char* trim_chars,
                      const char* expected);

#endif  // C2_S21_STRINGPLUS_0_TEST_COMMONS_H
