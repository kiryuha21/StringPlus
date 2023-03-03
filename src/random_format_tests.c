#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#include "s21_sprintf.h"
#include "s21_sscanf.h"
#include "s21_string.h"

typedef enum { SPRINTF, SSCANF } Funcs;
typedef enum {
  INT,
  UINT,
  DOUBLE,
  LDOUBLE,
  STRING,
  CHAR,
  WSTRING,
  WCHAR,
  INT_PTR,
  VOID_PTR,
  FLOAT
} Types;

const char* specifications_test = "cdioxXu%pnfsgGeE";
const char* writer_flags_test = "+- #0";
const char* lengths_test = "hlL";

char* generate_random_size_string(int* size) {
  *size = rand() % 20 + 5;
  char* res = (char*)calloc(*size + 1, sizeof(char));
  if (res != NULL) {
    for (int i = 0; i < *size; ++i) {
      res[i] = (char)((rand() % 97) + 30);
    }
  }
  return res;
}

wchar_t* generate_random_size_wstring(int* size) {
  char* string = generate_random_size_string(size);
  wchar_t* res = calloc(*size + 1, sizeof(wchar_t));
  mbstowcs(res, string, *size + 1);
  free(string);
  return res;
}

wchar_t generate_random_wchar() {
  char temp = (char)((rand() % 97) + 30);
  wchar_t res;
  mbtowc(&res, &temp, 1);
  return res;
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
    int my_num = 0;
    my_num = *(int*)val;

    my_ret = sprintf(my_res, format, (int*)val);
    int std_num = 0;
    std_num = *(int*)val;

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
    if (random_str == NULL) {
      free(str);
      return 1;
    }
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

char* random_format(int for_sprintf) {
  char specification = specifications_test[rand() % 16];
  char* format = calloc(100, sizeof(char));
  if (format == NULL) {
    return NULL;
  }

  int index = 0;
  format[index++] = '%';
  // flags
  if (for_sprintf) {
    for (int j = 0; rand() % 3 > 0 && j < 4; ++j) {
      format[index++] = writer_flags_test[rand() % 5];
    }
  }
  // width
  if (for_sprintf || s21_strchr("cp", specification) == NULL) {
    for (int j = 0; rand() % 2 == 0 && j < 2; ++j) {
      if (!for_sprintf && j == 0) {
        format[index++] = '4' + rand() % 6;
      } else {
        format[index++] = '0' + rand() % 9;
      }
    }
  }
  // precision
  if (for_sprintf) {
    if (rand() % 2) {
      format[index++] = '.';
      for (int j = 0; rand() % 2 == 0 && j < 1; ++j) {
        format[index++] = (char)('0' + rand() % 10);
      }
    }
  }
  // length
  if (rand() % 2) {
    if (s21_strchr("eEfgG", specification)) {
      format[index++] = 'L';
    } else if (strchr("cs", specification)) {
      format[index++] = 'l';
    } else if (for_sprintf || strchr("pn", specification) == NULL) {
      format[index++] = lengths_test[rand() % 2];
      if (rand() % 2 && strchr("ud", specification) != NULL && !for_sprintf) {
        format[index] = format[index - 1];
        ++index;
      }
    }
  }
  //  specification
  format[index++] = specification;
  return format;
}

// const char* specifications_test = "cdioxXu%pneEfsgG";
int random_test(int with_assert, int type) {
  char* format = random_format(type == SPRINTF ? 1 : 0);
  if (format == NULL) {
    return 1;
  }
  char specification = format[s21_strlen(format) - 1];
  WriterFormat writer;
  init_writer(&writer);
  parse_into_writer(&writer, format);
  validate_writer_flags(&writer);
  int cmp = 1;
  if (specification == 'c') {
    if (strchr(format, 'l')) {
      wchar_t res = generate_random_wchar();
      if (type == SPRINTF) {
        cmp = sprintf_test_common(format, (void*)(&res), WCHAR, with_assert);
      } else if (type == SSCANF) {
        cmp = sscanf_test_common(format, (void*)(&res), WCHAR, with_assert);
      }
    } else {
      char res = (char)((rand() % 97) + 30);
      if (type == SPRINTF) {
        cmp = sprintf_test_common(format, (void*)(&res), CHAR, with_assert);
      } else if (type == SSCANF) {
        cmp = sscanf_test_common(format, (void*)(&res), CHAR, with_assert);
      }
    }
  } else if (strchr("xuXo", specification)) {
    unsigned long long res = rand() - rand();
    if (type == SPRINTF) {
      cmp = sprintf_test_common(format, (void*)(&res), UINT, with_assert);
    } else if (type == SSCANF) {
      cmp = sscanf_test_common(format, (void*)(&res), UINT, with_assert);
    }
  } else if (strchr("di", specification)) {
    long long res = rand() - rand();
    if (type == SPRINTF) {
      cmp = sprintf_test_common(format, (void*)(&res), INT, with_assert);
    } else if (type == SSCANF) {
      cmp = sscanf_test_common(format, (void*)(&res), INT, with_assert);
    }
  } else if (s21_strchr("efEgG", specification)) {
    if (strchr(format, 'L')) {
      long double divider = (long double)(rand() % (int)pow(10, rand() % 10));
      long double divisor = (long double)(rand() % (int)pow(10, rand() % 10));
      long double res = divisor / divider;
      if (rand() % 2) {
        res *= -1;
      }
      if (type == SPRINTF) {
        cmp = sprintf_test_common(format, (void*)(&res), LDOUBLE, with_assert);
      } else if (type == SSCANF) {
        cmp = sscanf_test_common(format, (void*)(&res), LDOUBLE, with_assert);
      }
    } else if (strchr(format, 'l')) {
      double divider = (double)(rand() % (int)pow(10, rand() % 10));
      double divisor = (double)(rand() % (int)pow(10, rand() % 10));
      double res = divisor / divider;
      if (rand() % 2) {
        res *= -1;
      }
      if (type == SPRINTF) {
        cmp = sprintf_test_common(format, (void*)(&res), DOUBLE, with_assert);
      } else if (type == SSCANF) {
        cmp = sscanf_test_common(format, (void*)(&res), DOUBLE, with_assert);
      }
    } else {
      float divider = (float)(rand() % (int)pow(10, rand() % 10));
      float divisor = (float)(rand() % (int)pow(10, rand() % 10));
      float res = divisor / divider;
      if (rand() % 2) {
        res *= -1;
      }
      if (type == SPRINTF) {
        cmp = sprintf_test_common(format, (void*)(&res), FLOAT, with_assert);
      } else if (type == SSCANF) {
        cmp = sscanf_test_common(format, (void*)(&res), FLOAT, with_assert);
      }
    }
  } else if (specification == '%') {
    char res = '%';
    if (type == SPRINTF) {
      cmp = sprintf_test_common(format, (void*)(&res), CHAR, with_assert);
    } else if (type == SSCANF) {
      cmp = sscanf_test_common(format, (void*)(&res), CHAR, with_assert);
    }
  } else if (specification == 'n') {
    int res;
    if (type == SPRINTF) {
      cmp = sprintf_test_common(format, (void*)(&res), INT_PTR, with_assert);
    } else if (type == SSCANF) {
      cmp = sscanf_test_common(format, (void*)(&res), INT_PTR, with_assert);
    }
  } else if (specification == 's') {
    if (strchr(format, 'l')) {
      int size;
      wchar_t* res = generate_random_size_wstring(&size);
      if (res != NULL) {
        if (type == SPRINTF) {
          cmp = sprintf_test_common(format, (void*)(res), WSTRING, with_assert);
        } else if (type == SSCANF) {
          cmp = sscanf_test_common(format, (void*)(res), WSTRING, with_assert);
        }
        free(res);
      }
    } else {
      int size;
      char* res = generate_random_size_string(&size);
      if (res != NULL) {
        if (type == SPRINTF) {
          cmp = sprintf_test_common(format, (void*)(res), STRING, with_assert);
        } else if (type == SSCANF) {
          cmp = sscanf_test_common(format, (void*)(res), STRING, with_assert);
        }
        free(res);
      }
    }
  } else if (specification == 'p') {
    if (rand() % 100 != 0) {
      int size;
      char* res = generate_random_size_string(&size);
      if (res != NULL) {
        char* ptr = res + (rand() % size);
        if (type == SPRINTF) {
          cmp =
              sprintf_test_common(format, (void*)(&ptr), VOID_PTR, with_assert);
        } else if (type == SSCANF) {
          cmp =
              sscanf_test_common(format, (void*)(&ptr), VOID_PTR, with_assert);
        }
        free(res);
      }
    } else {
      if (type == SPRINTF) {
        cmp = sprintf_test_common(format, NULL, VOID_PTR, with_assert);
      } else if (type == SSCANF) {
        cmp = sscanf_test_common(format, NULL, VOID_PTR, with_assert);
      }
    }
  }

  free(format);
  return cmp;
}

void random_tests(int with_assert, int count, int type) {
  int cmp = 0;
  srand(time(NULL));
  for (int i = 0; i < count && cmp == 0; ++i) {
    printf("%d | %s\n", i + 1, type == SPRINTF ? "sprintf" : "sscanf");
    cmp = random_test(with_assert, type);
  }

  printf(cmp ? "ERROR\n" : "SUCCESS\n");
}

int main() {
  random_tests(0, 1000000, SPRINTF);
  random_tests(0, 1000000, SSCANF);
}
