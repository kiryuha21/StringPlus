#include "test_commons.h"

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

void print_sscanf(char* format, char* str, Types type, void* my_val,
                  void* std_val, int my_ret, int std_ret) {
  printf("---------\nformat:\n%s\nstr:\n%s\n", format, str);
  if (type == INT) {
    ReaderFormat reader;
    init_reader(&reader);
    parse_into_reader(&reader, format + 1);
    Lengths lens = reader.length;
    if (lens.l == 1 || lens.L == 1) {
      printf("my_val: %ld\nstd_val: %ld\n", *((long*)my_val),
             *((long*)std_val));
    } else if (lens.l >= 2 || lens.L >= 2) {
      printf("my_val: %lld\nstd_val: %lld\n", *((long long*)my_val),
             *((long long*)std_val));
    } else if (lens.h == 1) {
      printf("my_val: %hd\nstd_val: %hd\n", *((short*)my_val),
             *((short*)std_val));
    } else if (lens.h >= 2) {
      printf("my_val: %hhd\nstd_val: %hhd\n", *((char*)my_val),
             *((char*)std_val));
    } else {
      printf("my_val: %d\nstd_val: %d\n", *((int*)my_val), *((int*)std_val));
    }
  } else if (type == INT_PTR) {
    printf("my_val: %d\nstd_val: %d\n", *((int*)my_val), *((int*)std_val));
  } else if (type == STRING) {
    printf("my_val: %s\nstd_val: %s\n", (char*)my_val, (char*)std_val);
  } else if (type == WSTRING) {
    printf("my_val: %ls\nstd_val: %ls\n", (wchar_t*)my_val, (wchar_t*)std_val);
  } else if (type == CHAR) {
    printf("%c is %d\n", *str, *str);
    printf("my_val: %c (%d)\nstd_val: %c (%d)\n", *((char*)my_val),
           *((char*)my_val), *((char*)std_val), *((char*)std_val));
  } else if (type == WCHAR) {
    printf("my_val: %lc (%d)\nstd_val: %lc (%d)\n", *((wchar_t*)my_val),
           *((wchar_t*)my_val), *((wchar_t*)std_val), *((wchar_t*)std_val));
  } else if (type == DOUBLE) {
    printf("my_val: %f\nstd_val: %f\n", *((double*)my_val),
           *((double*)std_val));
  } else if (type == LDOUBLE) {
    printf("my_val: %Lf\nstd_val: %Lf\n", *((long double*)my_val),
           *((long double*)std_val));
  } else if (type == FLOAT) {
    printf("my_val: %f\nstd_val: %f\n", *((float*)my_val), *((float*)std_val));
  } else if (type == VOID_PTR) {
    printf("my_val: %p\nstd_val: %p\n", my_val, std_val);
  } else if (type == UINT) {
    ReaderFormat reader;
    init_reader(&reader);
    parse_into_reader(&reader, format + 1);
    Lengths lens = reader.length;
    if (lens.l == 1 || lens.L == 1) {
      printf("my_val: %lu\nstd_val: %lu\n", *((unsigned long*)my_val),
             *((unsigned long*)std_val));
    } else if (lens.l >= 2 || lens.L >= 2) {
      printf("my_val: %llu\nstd_val: %llu\n", *((unsigned long long*)my_val),
             *((unsigned long long*)std_val));
    } else if (lens.h == 1) {
      printf("my_val: %hu\nstd_val: %hu\n", *((unsigned short*)my_val),
             *((unsigned short*)std_val));
    } else if (lens.h >= 2) {
      printf("my_val: %hhu\nstd_val: %hhu\n", *((unsigned char*)my_val),
             *((unsigned char*)std_val));
    } else {
      printf("my_val: %u\nstd_val: %u\n", *((unsigned int*)my_val),
             *((unsigned int*)std_val));
    }
  }
  printf("my_ret = %d; std_ret = %d\n", my_ret, std_ret);
}

void print_sprintf(char* format, void* values, Types type, char* my_res,
                   char* std_res, int my_ret, int std_ret) {
  printf("---------\nformat:\n%s\n", format);
  if (type == INT || type == INT_PTR) {
    printf("val:\n%lld\n", *((long long int*)values));
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

int cmp_floats_by_str(char* format, char* str, void* my_val, void* std_val,
                      Types type) {
  WriterFormat writer;
  init_writer(&writer);
  parse_into_writer(&writer, format + 1);
  if (type == DOUBLE) {
    double delta = pow(0.1, define_precision_with_e(str)) * 5;
    if (fabs(*(double*)my_val - *(double*)std_val) > delta) {
      printf("\ndelta - %.*f my_val = %.*f; std_val = %.*f; diff = %.*f\n",
             writer.precision, delta, writer.precision, *(double*)my_val,
             writer.precision, *(double*)std_val, writer.precision,
             fabs(*(double*)my_val - *(double*)std_val));
      return 1;
    }
    return 0;
  } else if (type == LDOUBLE) {
    long double delta = pow(0.1, define_precision_with_e(str)) * 5;
    if (fabsl(*(long double*)my_val - *(long double*)std_val) > delta) {
      printf("\ndelta - %.*Lf my_val = %.*Lf; std_val = %.*Lf; diff = %.*Lf\n",
             writer.precision, delta, writer.precision, *(long double*)my_val,
             writer.precision, *(long double*)std_val, writer.precision,
             fabsl(*(long double*)my_val - *(long double*)std_val));
      return 1;
    }
    return 0;
  } else if (type == FLOAT) {
    float delta = powf(0.1f, (float)define_precision_with_e(str)) * 5;
    if (fabsl(*(float*)my_val - *(float*)std_val) > delta) {
      printf("\ndelta - %.*f my_val = %.*f; std_val = %.*f; diff = %.*f\n",
             writer.precision, delta, writer.precision, *(float*)my_val,
             writer.precision, *(float*)std_val, writer.precision,
             fabsf(*(float*)my_val - *(float*)std_val));
      return 1;
    }
    return 0;
  }
  return 1;
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
      print_sprintf(format, val, type, my_res, std_res, my_ret, std_ret);
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

  print_sprintf(format, val, type, my_res, std_res, my_ret, std_ret);
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

int sscanf_test_common(char* format, void* val, Types type, int with_assert) {
  char* str = calloc(10000, sizeof(char));
  if (str == NULL) {
    puts("Bad allocation");
    return 1;
  }

  int std_ret = 1, my_ret = 0, ret_val = 0;
  // TODO: handle
  // int is_g = !(strchr(format, 'g') != NULL || strchr(format, 'G') != NULL);
  if (type == INT) {
    ReaderFormat reader;
    init_reader(&reader);
    parse_into_reader(&reader, format + 1);
    Lengths lens = reader.length;
    if (lens.l == 1 || lens.L == 1) {
      long my_val, std_val;
      sprintf(str, format, *((long*)val));
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    } else if (lens.l >= 2 || lens.L >= 2) {
      long long my_val, std_val;
      sprintf(str, format, *((long long*)val));
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    } else if (lens.h == 1) {
      short int my_val, std_val;
      sprintf(str, format, *((short int*)val));
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    } else if (lens.h >= 2) {
      char my_val, std_val;
      sprintf(str, format, *((char*)val));
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    } else {
      int my_val, std_val;
      sprintf(str, format, *((int*)val));
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    }
  } else if (type == UINT) {
    ReaderFormat reader;
    init_reader(&reader);
    parse_into_reader(&reader, format + 1);
    Lengths lens = reader.length;
    if (lens.l == 1 || lens.L == 1) {
      unsigned long my_val, std_val;
      sprintf(str, format, *((unsigned long*)val));
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    } else if (lens.l >= 2 || lens.L >= 2) {
      unsigned long long my_val, std_val;
      sprintf(str, format, *((unsigned long long*)val));
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    } else if (lens.h == 1) {
      unsigned short my_val, std_val;
      sprintf(str, format, *((unsigned short*)val));
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    } else if (lens.h >= 2) {
      unsigned char my_val, std_val;
      sprintf(str, format, *((unsigned char*)val));
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    } else {
      unsigned int my_val, std_val;
      sprintf(str, format, *((unsigned int*)val));
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    }
  } else if (type == STRING) {
    char *my_val = calloc(10000, sizeof(my_val)),
         *std_val = calloc(10000, sizeof(std_val));
    if (my_val != NULL && std_val != NULL) {
      sprintf(str, format, (char*)val);
      std_ret = sscanf(str, format, std_val);
      my_ret = s21_sscanf(str, format, my_val);
      if (s21_strcmp(my_val, std_val) != 0) {
        ret_val = 1;
        sprintf(str, format, (char*)val);
        std_ret = sscanf(str, format, std_val);
        my_ret = s21_sscanf(str, format, my_val);
      }
      print_sscanf(format, str, type, (void*)(my_val), (void*)(std_val), my_ret,
                   std_ret);
    }
    free(std_val);
    free(my_val);
  } else if (type == WSTRING) {
    wchar_t *my_val = calloc(10000, sizeof(wchar_t)),
            *std_val = calloc(10000, sizeof(wchar_t));
    if (my_val != NULL && std_val != NULL) {
      sprintf(str, format, (wchar_t*)val);
      std_ret = sscanf(str, format, std_val);
      my_ret = s21_sscanf(str, format, my_val);
      if (wcscmp(my_val, std_val) != 0) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(my_val), (void*)(std_val), my_ret,
                   std_ret);
    }
    free(std_val);
    free(my_val);
  } else if (type == CHAR) {
    sprintf(str, format, *((char*)val));
    if (*((char*)val) != '%') {
      char my_val, std_val;
      std_ret = sscanf(str, format, &std_val);
      my_ret = s21_sscanf(str, format, &my_val);
      if (my_val != std_val) {
        ret_val = 1;
      }
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    } else {
      char my_val = '%', std_val = '%';
      std_ret = sscanf(str, "%%");
      my_ret = s21_sscanf(str, "%%");
      print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val),
                   my_ret, std_ret);
    }
  } else if (type == WCHAR) {
    wchar_t my_val, std_val;
    sprintf(str, format, *((wchar_t*)val));
    std_ret = sscanf(str, format, &std_val);
    my_ret = s21_sscanf(str, format, &my_val);
    if (my_val != std_val) {
      ret_val = 1;
    }
    print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val), my_ret,
                 std_ret);
  } else if (type == INT_PTR) {
    int size;
    char* random_str = generate_random_size_string(&size);
    for (int i = 0; i < size; ++i) {
      if (random_str[i] == '%' || random_str[i] == ' ') {
        ++random_str[i];
      }
    }

    char* temp = (char*)calloc(size + 20, sizeof(char));
    s21_strncpy(temp, random_str, rand() % size + 1);
    s21_strcat(temp, format);
    int my_val, std_val;
    std_ret = sscanf(random_str, temp, &std_val);
    my_ret = s21_sscanf(random_str, temp, &my_val);
    if (my_val != std_val) {
      ret_val = 1;
    }
    print_sscanf(temp, random_str, type, (void*)(&my_val), (void*)(&std_val),
                 my_ret, std_ret);
    free(temp);
    free(random_str);
  } else if (type == VOID_PTR) {
    void *my_val, *std_val;
    sprintf(str, format, val);
    std_ret = sscanf(str, format, &std_val);
    my_ret = s21_sscanf(str, format, &my_val);
    if (my_val != std_val) {
      ret_val = 1;
    }
    print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val), my_ret,
                 std_ret);
  } else if (type == DOUBLE) {
    double my_val, std_val;
    sprintf(str, format, *((double*)val));
    std_ret = sscanf(str, format, &std_val);
    my_ret = s21_sscanf(str, format, &my_val);
    ret_val = cmp_floats_by_str(format, str, (void*)(&my_val),
                                (void*)(&std_val), DOUBLE);

    print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val), my_ret,
                 std_ret);
  } else if (type == LDOUBLE) {
    long double my_val, std_val;
    sprintf(str, format, *((long double*)val));
    std_ret = sscanf(str, format, &std_val);
    my_ret = s21_sscanf(str, format, &my_val);
    ret_val = cmp_floats_by_str(format, str, (void*)(&my_val),
                                (void*)(&std_val), LDOUBLE);

    print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val), my_ret,
                 std_ret);
  } else if (type == FLOAT) {
    float my_val, std_val;
    sprintf(str, format, *((float*)val));
    std_ret = sscanf(str, format, &std_val);
    my_ret = s21_sscanf(str, format, &my_val);
    ret_val = cmp_floats_by_str(format, str, (void*)(&my_val),
                                (void*)(&std_val), FLOAT);

    print_sscanf(format, str, type, (void*)(&my_val), (void*)(&std_val), my_ret,
                 std_ret);
  }

  free(str);
  if (std_ret != my_ret) {
    ret_val = 1;
  }
  if (with_assert) {
    ck_assert(!ret_val);
  }

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
