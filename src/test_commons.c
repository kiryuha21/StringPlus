#include "test_commons.h"

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_debug(char* format, void* values, Types type, char* my_res,
                 char* std_res, int my_ret, int std_ret) {
  printf("---------\nformat:\n%s\n", format);
  if (type == INT || type == INT_PTR) {
    printf("val:\n%d\n", *((int*)values));
  } else if (type == STRING) {
    printf("val:\n%s\n", (char*)values);
  } else if (type == WSTRING) {
    printf("val:\n\"%ls\"\n", (wchar_t*)values);
  } else if (type == CHAR) {
    printf("val:\n\"%c\"\n", *((char*)values));
  } else if (type == WCHAR) {
    printf("val:\n\"%lc\"\n", *((wchar_t*)values));
  } else if (type == DOUBLE) {
    printf("val:\n%f\n", *((double*)values));
  } else if (type == LDOUBLE) {
    printf("val:\n%Lf\n", *((long double*)values));
  } else if (type == VOID_PTR) {
    printf("val:\n%p\n", values);
  }
  printf("results(my - std):\n\"%s\"\n\"%s\"\n", my_res, std_res);
  printf("returns(my - std):\n%d\n%d\n---------\n", my_ret, std_ret);
}

int define_precision_with_e(const char* str) {
  int res = 0;
  for (; *str && *str != '.' && *str != 'e' && *str != 'E'; ++str)
    ;
  if (*str == '.') {
    ++str;
    for (; *str >= '0' && *str <= '9'; ++str, ++res)
      ;
  }
  if (*str == 'e' || *str == 'E') {
    ++str;
    res -= atoi(str);
  }
  return res;
}

int test_float_types(char* format, char* my_res, char* std_res, int my_ret,
                     int std_ret, Types type) {
  if (my_ret != std_ret || strcmp(my_res, std_res) == 0) {
    free(my_res);
    free(std_res);
    if (my_ret != std_ret) {
      return 1;
    }
    return 0;
  }

  WriterFormat writer;
  init_writer(&writer);
  parse_into_writer(&writer, format + 1);

  if (type == DOUBLE) {
    double my_num = strtod(my_res, NULL);
    double std_num = strtod(std_res, NULL);
    double delta = pow(0.1, define_precision_with_e(my_res)) * 5;
    if (fabs(my_num - std_num) > delta) {
      printf("\ndelta - %.*f num1 - %.*f num2 - %.*f num2 - num1 = %.*f\n",
             writer.precision, delta, writer.precision, my_num,
             writer.precision, std_num, writer.precision,
             fabs(std_num - my_num));
    }
    free(my_res);
    free(std_res);
    return fabs(my_num - std_num) <= delta ? 0 : 1;
  }
  long double my_num = strtold(my_res, NULL);
  long double std_num = strtold(std_res, NULL);
  long double delta = powl(0.1, define_precision_with_e(my_res)) * 5;
  if (fabsl(my_num - std_num) > delta) {
    printf("\ndelta - %.*Lf num1 - %.*Lf num2 - %.*Lf\n", writer.precision,
           delta, writer.precision, my_num, writer.precision, std_num);
  }
  free(my_res);
  free(std_res);
  return fabsl(my_num - std_num) <= delta ? 0 : 1;
}

int sprintf_test_common(char* format, void* val, Types type, int with_assert) {
  char* my_res = calloc(10001, sizeof(char));
  char* std_res = calloc(10000, sizeof(char));
  if (my_res == NULL) {
    if (std_res != NULL) {
      free(std_res);
    }
    puts("Bad allocation");
    return 1;
  }
  if (std_res == NULL) {
    free(my_res);
    puts("Bad allocation");
    return 1;
  }

  int is_g = !(strchr(format, 'g') != NULL || strchr(format, 'G') != NULL);
  int my_ret = 0, std_ret = 0;
  if (type == INT) {
    std_ret = sprintf(std_res, format, *((int*)val));
    my_ret = s21_sprintf(my_res, format, *((int*)val));
  } else if (type == STRING) {
    std_ret = sprintf(std_res, format, (char*)val);
    my_ret = s21_sprintf(my_res, format, (char*)val);
  } else if (type == WSTRING) {
    std_ret = sprintf(std_res, format, (wchar_t*)val);
    my_ret = s21_sprintf(my_res, format, (wchar_t*)val);
  } else if (type == CHAR) {
    std_ret = sprintf(std_res, format, *((char*)val));
    my_ret = s21_sprintf(my_res, format, *((char*)val));
  } else if (type == WCHAR) {
    std_ret = sprintf(std_res, format, *((wchar_t*)val));
    my_ret = s21_sprintf(my_res, format, *((wchar_t*)val));
  } else if (type == DOUBLE) {
    std_ret = sprintf(std_res, format, *((double*)val));
    my_ret = s21_sprintf(my_res, format, *((double*)val));
  } else if (type == LDOUBLE) {
    std_ret = sprintf(std_res, format, *((long double*)val));
    my_ret = s21_sprintf(my_res, format, *((long double*)val));
  } else if (type == INT_PTR) {  // only for %n specifier
    std_ret = s21_sprintf(std_res, format, (int*)val);
    int my_num = *(int*)val;

    my_ret = sprintf(my_res, format, (int*)val);
    int std_num = *(int*)val;

    if (my_num != std_num) {
      printf("\nmy num - %d, std num - %d\n", my_num, std_num);
      print_debug(format, val, type, my_res, std_res, my_ret, std_ret);
      free(my_res);
      free(std_res);
      if (with_assert) {
        ck_assert(1);
      }
      return 1;
    }
  } else if (type == VOID_PTR) {
    std_ret = sprintf(std_res, format, val);
    my_ret = s21_sprintf(my_res, format, val);
  }

  if (s21_strcmp(my_res, std_res) || my_ret != std_ret) {
    print_debug(format, val, type, my_res, std_res, my_ret, std_ret);
  }
#ifdef DEBUG
  else {
    print_debug(format, val, type, my_res, std_res, my_ret, std_ret);
  }
#endif
  if (type == DOUBLE || type == LDOUBLE) {
    int ret_val =
        test_float_types(format, my_res, std_res, my_ret, std_ret, type) &&
        is_g;
    if (with_assert) {
      ck_assert(!ret_val);
    }
    return ret_val;
  }
  int ret_val = s21_strcmp(my_res, std_res) || my_ret != std_ret;
  if (with_assert) {
    ck_assert(!ret_val);
  }
  free(my_res);
  free(std_res);
  return ret_val;
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
