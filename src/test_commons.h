#ifndef C2_S21_STRINGPLUS_0_TEST_COMMONS_H
#define C2_S21_STRINGPLUS_0_TEST_COMMONS_H

#include "s21_string.h"

void strtok_test_common(char s21_haystack[], char str_haystack[], char delim[]);
void strlen_test_common(char* str);
void strspn_test_common(char* haystack, char* entries, int should_be_in);
void memcmp_test_common(char* first, char* second, size_t len);

#endif  // C2_S21_STRINGPLUS_0_TEST_COMMONS_H
