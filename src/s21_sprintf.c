#include "s21_sprintf.h"

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

extern const char *specifications;
extern const char *writer_flags;
extern const char *lengths;
extern const char *decimal_places;

void init_extra_info(ExtraInfo *info, s21_size_t written_bytes, int *bad_return,
                     int *null_chars) {
  info->written_bytes = written_bytes;
  info->bad_return = bad_return;
  info->null_chars = null_chars;
  info->flags_applicable = 1;
  info->left_space = UNKNOWN;
}

void init_flags(Flags *flags) {
  flags->lattice_flag = 0;
  flags->minus_flag = 0;
  flags->plus_flag = 0;
  flags->space_flag = 0;
  flags->zero_flag = 0;
}

void init_writer(WriterFormat *writer) {
  init_flags(&writer->flags);
  init_lengths(&writer->length);
  writer->precision = UNKNOWN;
  writer->width = UNKNOWN;
  writer->specification = UNKNOWN;
  writer->parsed_length = 0;
}

void validate_writer_flags(WriterFormat *writer) {
  if (writer->flags.plus_flag > 1) {
    writer->flags.plus_flag = 1;
  }

  if (writer->flags.minus_flag) {
    writer->flags.zero_flag = 0;
  }

  if (writer->flags.plus_flag && writer->specification != 'p') {
    writer->flags.space_flag = 0;
  }

  if (writer->specification != UNKNOWN) {
    if (writer->precision != UNKNOWN &&
        s21_strchr("puoxXid", writer->specification)) {
      writer->flags.zero_flag = 0;
    }

    if (!s21_strchr("oxXeEfugG", writer->specification)) {
      writer->flags.lattice_flag = 0;
    }

    if ((writer->flags.zero_flag || writer->flags.space_flag) &&
        s21_strchr("cs%", writer->specification)) {
      writer->flags.zero_flag = 0;
    }

    if (s21_strchr("oxXcus%", writer->specification)) {
      writer->flags.plus_flag = 0;
      writer->flags.space_flag = 0;
    }
  }
}

void parse_into_writer(WriterFormat *writer, const char *src) {
  // flags
  while (s21_strchr(writer_flags, src[writer->parsed_length]) != S21_NULL) {
    if (src[writer->parsed_length] == '-') {
      ++writer->flags.minus_flag;
    } else if (src[writer->parsed_length] == '+') {
      ++writer->flags.plus_flag;
    } else if (src[writer->parsed_length] == ' ') {
      ++writer->flags.space_flag;
    } else if (src[writer->parsed_length] == '#') {
      ++writer->flags.lattice_flag;
    } else if (src[writer->parsed_length] == '0') {
      ++writer->flags.zero_flag;
    }

    ++writer->parsed_length;
  }

  // width
  if (src[writer->parsed_length] == '*') {
    writer->width = ASTERISK;
    ++writer->parsed_length;
  } else {
    int width = str_to_int(&src[writer->parsed_length], &writer->parsed_length);
    if (width != -1) {
      writer->width = width;
    }
  }

  // precision
  if (src[writer->parsed_length] == '.') {
    ++writer->parsed_length;
    if (src[writer->parsed_length] == '*') {
      writer->precision = ASTERISK;
      ++writer->parsed_length;
    } else {
      int precision =
          str_to_int(&src[writer->parsed_length], &writer->parsed_length);
      if (precision != -1) {
        writer->precision = precision;
      } else {
        writer->precision = EMPTY;
      }
    }
  }

  // length
  while (s21_strchr(lengths, src[writer->parsed_length]) != S21_NULL) {
    if (src[writer->parsed_length] == 'L') {
      ++writer->length.L;
    } else if (src[writer->parsed_length] == 'l') {
      ++writer->length.l;
    } else if (src[writer->parsed_length] == 'h') {
      ++writer->length.h;
    }

    ++writer->parsed_length;
  }

  // specification
  if (s21_strchr(specifications, src[writer->parsed_length]) != S21_NULL) {
    writer->specification = src[writer->parsed_length];
    ++writer->parsed_length;
  }
}

int get_digits_amount(unsigned long long num, int number_system) {
  if (num == 0) {
    return 1;
  }

  return (int)floorl(log2l((num)) / log2l(number_system)) + 1;
}

char *convert_ll_to_string(ull num, int number_system) {
  int len = get_digits_amount(num, number_system);

  char *str = (char *)calloc(len + 4, sizeof(char));

  if (str != S21_NULL) {
    if (num == 0) {
      *str = '0';
    } else {
      for (int i = len - 1; i >= 0 && num > 0; --i, num /= number_system) {
        str[i] = (char)(decimal_places[num % number_system]);
      }
    }

    if (*str == '\0') {
      char *temp = str + 1;
      while (*temp) {
        *(temp - 1) = *temp;
        ++temp;
      }
      *(temp - 1) = *temp;
    }
  }

  return str;
}

void safe_replace(char **dst, char **replacer) {
  if (*dst != S21_NULL) {
    free(*dst);
  }
  *dst = *replacer;
}

long long round_ldouble(long double num, int precision) {
  long double enlarged = num * powl(10.0, precision);
  return llrintl(enlarged);
}

long long round_double(double num, int precision) {
  double enlarged = num * pow(10.0, precision);
  return llrint(enlarged);
}

long long handle_overflow(long long int num, WriterFormat *writer) {
  if (num < 0) {
    writer->flags.plus_flag = -1;
  }
  return llabs(num);
}

void signed_length_wrapper(WriterFormat *writer, long long num,
                           int number_system, char **formatted_string) {
  long long temp = apply_signed_length(&writer->length, num);
  long long res = handle_overflow(temp, writer);
  *formatted_string = convert_ll_to_string(res, number_system);
}

void unsigned_length_wrapper(WriterFormat *writer, ull num, int number_system,
                             char **formatted_string) {
  ull temp = apply_unsigned_length(&writer->length, num);
  *formatted_string = convert_ll_to_string(temp, number_system);
}

int get_ldouble_pow(long double *num) {
  int pow = 0;
  for (; *num >= 10; ++pow) {
    (*num) /= 10;
  }
  for (; *num < 1 && *num != 0; --pow) {
    (*num) *= 10;
  }
  return pow;
}

int get_double_pow(double *num) {
  int pow = 0;
  for (; *num >= 10; ++pow) {
    (*num) /= 10;
  }
  for (; *num < 1 && *num != 0; --pow) {
    (*num) *= 10;
  }
  return pow;
}

void inf_nan_handle(char **fstring, char *str, ExtraInfo *info) {
  *fstring = (char *)calloc(4, sizeof(char));
  s21_strcpy(*fstring, str);
  info->flags_applicable = 0;
}

void handle_inf(char **fstring, char spec, ExtraInfo *info) {
  inf_nan_handle(fstring, s21_strchr("EG", spec) ? "INF" : "inf", info);
}

void handle_nan(char **fstring, char spec, ExtraInfo *info) {
  inf_nan_handle(fstring, s21_strchr("EG", spec) ? "NAN" : "nan", info);
}

void handle_exp_part(char **fstring, char spec, int rounded_pow) {
  if (s21_strchr("eE", spec) == S21_NULL) {
    return;
  }

  int power = rounded_pow;
  int add_len = (rounded_pow <= -100 || rounded_pow >= 100) ? 3 : 2;

  s21_size_t strlen = s21_strlen(*fstring) + 1;
  s21_size_t endlen = strlen + add_len;
  for (; endlen > strlen; --endlen) {
    (*fstring)[endlen] = (char)('0' + abs(rounded_pow % 10));
    rounded_pow /= 10;
  }
  (*fstring)[endlen--] = (char)(power < 0 ? '-' : '+');
  (*fstring)[endlen] = spec == 'e' ? 'e' : 'E';
}

int define_precision(int precision) {
  int result = precision == UNKNOWN ? DEFAULT_PRECISION : precision;
  if (precision == EMPTY) {
    result = 0;
  }
  return result;
}

void float_to_str(char **fstring, int len, int precision, int lattice_flag,
                  long long rounded) {
  for (int i = len; i >= 0; --i) {
    if (i == len - precision) {
      if (precision != 0 || lattice_flag) {
        (*fstring)[i] = '.';
      }
    } else {
      (*fstring)[i] = (char)('0' + rounded % 10);
      rounded /= 10;
    }
  }
}

s21_size_t wchar_strlen(const wchar_t *str) {
  s21_size_t count = 0;
  if (str != S21_NULL) {
    for (const wchar_t *sym = str; *sym != L'\0'; ++sym, ++count) {
    }
  }
  return count;
}

int is_valid_string(char *str) {
  s21_size_t len = s21_strlen(str);
  for (s21_size_t i = 0; i < len; ++i) {
    if (str[i] < 0) {
      return 0;
    }
  }
  return 1;
}

void handle_null_char(ExtraInfo *info, WriterFormat *writer) {
  *(info->null_chars) +=
      (writer->flags.minus_flag && writer->width != UNKNOWN ? writer->width
                                                            : 1);
  writer->width = (writer->width == UNKNOWN || writer->flags.minus_flag
                       ? UNKNOWN
                       : writer->width - 1);
}

char *rfind_ch(char *str, char ch) {
  char *res = *str == ch ? str : S21_NULL;
  int not_found = 1;
  for (char *str_end = str + s21_strlen(str); str != str_end && not_found;
       --str_end) {
    if (*str_end == ch) {
      res = str_end;
      not_found = 0;
    }
  }

  return res;
}

char *rfind_str(char *str, const char *chars) {
  char *res = rfind_ch(str, *chars);
  for (const char *ch = chars + 1; *ch; ++ch) {
    char *new_find = rfind_ch(str, *ch);
    if (new_find && new_find > res) {
      res = new_find;
    }
  }
  return res;
}

char *rfind_str_before_sym(char *str, const char *chars,
                           const char *before_chars) {
  char *end = rfind_str(str, before_chars);
  char end_ch = '\0';
  if (end) {
    end_ch = *end;
    *end = '\0';
  }
  char *res = S21_NULL;
  for (const char *ch = chars; *ch; ++ch) {
    char *new_find = rfind_ch(str, *ch);
    if (new_find && new_find > res) {
      res = new_find;
    }
  }
  if (end) {
    *end = end_ch;
  }
  return res;
}

int build_base(char **formatted_string, WriterFormat *writer, ExtraInfo *info,
               va_list vars) {
  if (s21_strchr("di", writer->specification)) {
    long long num = va_arg(vars, long long);

    signed_length_wrapper(writer, num, 10, formatted_string);

    if (*formatted_string != S21_NULL && **formatted_string == '0') {
      *(info->bad_return) = 0;
    }

  } else if (s21_strchr("oxXu", writer->specification)) {
    unsigned long long num = va_arg(vars, unsigned long long);

    int number_system = 10;
    if (s21_strchr("xX", writer->specification)) {
      number_system = 16;
    } else if (s21_strchr("o", writer->specification)) {
      number_system = 8;
    }

    unsigned_length_wrapper(writer, num, number_system, formatted_string);

    if (s21_strchr("x", writer->specification)) {
      char *temp = s21_to_lower(*formatted_string);
      safe_replace(formatted_string, &temp);
    }

    if (**formatted_string == '0') {
      if (writer->specification != 'o') {
        writer->flags.lattice_flag = 0;
      }
      *(info->bad_return) = 0;
    }
  } else if (writer->specification == '%') {
    writer->width = UNKNOWN;
    writer->specification = 'c';
    *formatted_string = (char *)calloc(2, sizeof(char));
    if (*formatted_string == S21_NULL) {
      return FAIL;
    }

    *formatted_string[0] = '%';
  } else if (writer->specification == 'c') {
    if (writer->length.l) {
      wchar_t num = va_arg(vars, wchar_t);
      *formatted_string = (char *)calloc(2, sizeof(wchar_t));
      if (*formatted_string == S21_NULL) {
        return FAIL;
      }

      if (num == L'\0') {
        handle_null_char(info, writer);
      }

      wchar_t temp[2] = {L'\0'};
      temp[0] = num;
      wcstombs(*formatted_string, temp, 2);
    } else {
      int num = va_arg(vars, int);
      *formatted_string = (char *)calloc(2, sizeof(char));
      if (*formatted_string == S21_NULL) {
        return FAIL;
      }

      if (num == '\0') {
        handle_null_char(info, writer);
      }

      (*formatted_string)[0] = (char)num;
    }
  } else if (s21_strchr("eEfgG", writer->specification)) {
    long double lnum = 0;
    double num = 0;

    if (writer->length.L) {
      lnum = va_arg(vars, long double);
    } else {
      num = va_arg(vars, double);
    }

    if (signbit(num) != 0 ||
        signbit(lnum) != 0) {  // better than "if (num < 0)"
      writer->flags.plus_flag = -1;
      if (writer->length.L) {
        lnum = fabsl(lnum);
      } else {
        num = fabs(num);
      }
    }

    if (isinf(num) || isinf(lnum)) {
      handle_inf(formatted_string, writer->specification, info);
      return OK;
    }

    if (isnan(num) || isnan(lnum)) {
      handle_nan(formatted_string, writer->specification, info);
      return OK;
    }

    int g_spec = s21_strchr("gG", writer->specification) ? 1 : 0;

    int precision = define_precision(writer->precision);
    long double lcp_num = lnum;
    double cp_num = num;

    if (s21_strchr("gG", writer->specification)) {
      int cp_pow = 0;
      if (writer->length.L) {
        cp_pow = get_ldouble_pow(&lcp_num);
      } else {
        cp_pow = get_double_pow(&cp_num);
      }
      if (precision == 0) {
        precision = 1;
      }
      if (cp_pow >= -4 && cp_pow < precision) {
        writer->specification = 'f';
        precision = precision - cp_pow - 1;
      } else {
        writer->specification = writer->specification == 'g' ? 'e' : 'E';
        precision = precision - 1;
      }
    }

    int rounded_pow = 0;
    if (s21_strchr("eE", writer->specification)) {
      if (writer->length.L) {
        rounded_pow = get_ldouble_pow(&lnum);
      } else {
        rounded_pow = get_double_pow(&num);
      }
    }

    long long rounded = 0;
    if (writer->length.L) {
      rounded = round_ldouble(lnum, precision);
    } else {
      rounded = round_double(num, precision);
    }

    int len = get_digits_amount(rounded, 10);
    if (s21_strchr("eE", writer->specification) &&
        rounded >= (long long)pow(10, precision + 1)) {
      --len;
      ++rounded_pow;
      rounded /= 10;
    }
    if (precision + 1 > len) {
      len = precision + 1;
    }

    *formatted_string = (char *)calloc(len + 6, sizeof(char));
    float_to_str(formatted_string, len, precision, writer->flags.lattice_flag,
                 rounded);
    handle_exp_part(formatted_string, writer->specification, rounded_pow);

    if (g_spec == 1) {
      char *num_end = *formatted_string + s21_strlen(*formatted_string);
      if (s21_strpbrk(*formatted_string, "eE")) {
        for (; *(num_end - 1) && *num_end != 'e' && *num_end != 'E'; --num_end)
          ;
      }
      char *point = rfind_ch(*formatted_string, '.');
      char *last_digit =
          rfind_str_before_sym(*formatted_string, "123456789", "eE");
      if (point) {
        if (last_digit && point < last_digit) {
          char *str = last_digit + 1;
          for (; *num_end; ++str, ++num_end) {
            *str = *num_end;
          }
          for (; *str; ++str) {
            *str = '\0';
          }
        } else {
          char *str = point;
          if (writer->flags.lattice_flag) {
            ++str;
          }
          for (; *num_end; ++str, ++num_end) {
            *str = *num_end;
          }
          for (; *str; ++str) {
            *str = '\0';
          }
        }
      }
    }
  } else if (writer->specification == 's') {
    if (writer->length.l) {
      wchar_t *wstring = va_arg(vars, wchar_t *);
      s21_size_t len = wchar_strlen(wstring);
      *formatted_string = (char *)calloc(len + 1, sizeof(wchar_t));
      if (wcstombs(*formatted_string, wstring, len + 1) == (s21_size_t)(-1)) {
        return FAIL;
      }
    } else {
      char *string = va_arg(vars, char *);
      if (!is_valid_string(string)) {
        return FAIL;
      }

      *formatted_string = (char *)calloc(s21_strlen(string) + 6, sizeof(char));
      if (string != NULL) {
        s21_strcpy(*formatted_string, string);
      } else if ((writer->precision != UNKNOWN && writer->precision >= 7)) {
        s21_strcpy(*formatted_string, "(null)");
      }
    }
  } else if (writer->specification == 'p') {
    writer->length.l = 1;
    writer->flags.lattice_flag = 1;

    void *pointer = va_arg(vars, void *);
    if (pointer == S21_NULL) {
      *formatted_string = (char *)calloc(6, sizeof(char));
      char *temp = s21_insert(*formatted_string, "(nil)", 0);
      safe_replace(formatted_string, &temp);
      writer->flags.lattice_flag = 0;
      writer->flags.plus_flag = 0;
      writer->flags.zero_flag = 0;
      writer->flags.space_flag = 0;
      writer->precision = UNKNOWN;
      return OK;
    }
    unsigned_length_wrapper(writer, (ull)pointer, 16, formatted_string);

    char *lowered = s21_to_lower(*formatted_string);
    safe_replace(formatted_string, &lowered);
  } else if (writer->specification == 'n') {
    int *num = va_arg(vars, int *);
    *num = (int)info->written_bytes;
    *(info->bad_return) = 0;
  }
  return OK;
}

s21_size_t apply_width(char **formatted_string, WriterFormat *writer) {
  s21_size_t res = 0;
  if (writer->width != UNKNOWN && writer->specification != 'n') {
    int len = (int)s21_strlen(*formatted_string);
    if (writer->width > len) {
      s21_size_t diff = writer->width - len;

      char *spacer = (char *)calloc(diff + 1, sizeof(char));
      s21_memset(spacer, ' ', diff);

      char *with_spacer;
      if (writer->flags.minus_flag) {
        with_spacer = s21_insert(spacer, *formatted_string, 0);
      } else {
        with_spacer = s21_insert(*formatted_string, spacer, 0);
      }
      res = s21_strlen(spacer);
      safe_replace(formatted_string, &with_spacer);
      free(spacer);
    }
  }
  return res;
}

void insert_null_spacer_at_index(char **src, int index, int nsp_size) {
  char *null_spacer = (char *)calloc(nsp_size + 1, sizeof(char));
  s21_memset(null_spacer, '0', nsp_size);

  char *result = s21_insert(*src, null_spacer, index);
  safe_replace(src, &result);
  free(null_spacer);
}

void apply_precision(char **formatted_string, WriterFormat *writer,
                     ExtraInfo *info) {
  if (writer->precision != UNKNOWN) {
    if (s21_strchr("iduoxX", writer->specification)) {
      if ((writer->precision != EMPTY && writer->precision != 0) ||
          **formatted_string != '0') {
        s21_size_t len = s21_strlen(*formatted_string);
        if (writer->precision >= (int)len) {
          char *trimmed = s21_trim(*formatted_string, " ");
          safe_replace(formatted_string, &trimmed);

          int diff = writer->precision - (int)s21_strlen(*formatted_string);
          insert_null_spacer_at_index(formatted_string, 0, diff);
        }
      } else {
        **formatted_string = '\0';
        info->bad_return = 0;
      }
    } else if (s21_strchr("iduxXs", writer->specification)) {
      int precision = define_precision(writer->precision);
      if (precision < (int)s21_strlen(*formatted_string)) {
        if (precision == 0) {
          *info->bad_return = 0;
        }
        char *cutted = (char *)calloc(precision + 1, sizeof(char));
        s21_strncpy(cutted, *formatted_string, precision);
        safe_replace(formatted_string, &cutted);
      }
    } else if (writer->specification == 'p') {
      if (writer->precision > (int)s21_strlen(*formatted_string)) {
        int diff = writer->precision - (int)s21_strlen(*formatted_string);
        insert_null_spacer_at_index(formatted_string, 0, diff);
      }
    }
  }
}

void add_to_num(char **formatted_string, const char *str, int reverse,
                s21_size_t left_space) {
  if (!reverse) {
    s21_size_t len = s21_strlen(str);
    if (len > left_space) {
      char *null_spaces = (char *)calloc(len - left_space + 1, sizeof(char));
      s21_memset(null_spaces, ' ', len - left_space);
      char *additional_space = s21_insert(*formatted_string, null_spaces, 0);
      safe_replace(formatted_string, &additional_space);
      for (s21_size_t i = 0; i < len; ++i) {
        (*formatted_string)[i] = str[i];
      }
      free(null_spaces);
    } else {
      s21_size_t start = 0;
      for (; (*formatted_string)[start + len] == ' '; ++start) {
      }
      for (s21_size_t i = 0; i < len; ++i, ++start) {
        (*formatted_string)[start] = str[i];
      }
    }
  } else {
    s21_size_t len = s21_strlen(str);
    if (len > left_space) {
      char *null_spaces = (char *)calloc(len - left_space + 1, sizeof(char));
      s21_memset(null_spaces, ' ', len - left_space);
      char *additional_space = s21_insert(*formatted_string, null_spaces,
                                          s21_strlen(*formatted_string));
      safe_replace(formatted_string, &additional_space);
      free(null_spaces);
    }
    s21_size_t i = 0;
    for (; (*formatted_string)[i] != ' ' && (*formatted_string); ++i)
      ;
    i += len - 1;
    for (; i >= len; --i) {
      (*formatted_string)[i] = (*formatted_string)[i - len];
    }
    for (i = 0; i < len; ++i) {
      (*formatted_string)[i] = str[i];
    }
  }
}

void apply_flags(char **formatted_string, WriterFormat *writer,
                 ExtraInfo *info) {
  if (s21_strchr("eEfpuoXxid", writer->specification) &&
      info->flags_applicable) {
    if (writer->flags.zero_flag) {
      char *str = *formatted_string;
      for (; str != S21_NULL && *str == ' '; ++str) {
        *str = '0';
      }
    }
  }

  if (writer->flags.lattice_flag) {
    // check if 0 is already in the right place
    if (writer->specification == 'o') {
      char *already = s21_strchr(*formatted_string, '0');
      int first = already == S21_NULL ? 0 : 1;
      if (first) {
        for (char *ptr = *formatted_string; ptr != S21_NULL && ptr != already;
             ++ptr) {
          if (*ptr != ' ' && *ptr != '0') {
            first = 0;
          }
        }
      }
      // if it's not
      if (!first) {
        add_to_num(formatted_string, "0", writer->flags.minus_flag,
                   info->left_space);
      }
    } else if (writer->specification == 'x' || writer->specification == 'p') {
      if (writer->flags.plus_flag == 1) {
        add_to_num(formatted_string, "+0x", writer->flags.minus_flag,
                   info->left_space);
      } else if (writer->flags.space_flag) {
        add_to_num(formatted_string, " 0x", writer->flags.minus_flag,
                   info->left_space);
      } else {
        add_to_num(formatted_string, "0x", writer->flags.minus_flag,
                   info->left_space);
      }
    } else if (writer->specification == 'X') {
      add_to_num(formatted_string, "0X", writer->flags.minus_flag,
                 info->left_space);
    }
  }

  if (writer->specification != 'n' && writer->specification != 'p') {
    if (writer->flags.plus_flag == 1) {
      add_to_num(formatted_string, "+", writer->flags.minus_flag,
                 info->left_space);
    } else if (writer->flags.plus_flag == -1) {
      add_to_num(formatted_string, "-", writer->flags.minus_flag,
                 info->left_space);
    } else if (writer->flags.space_flag) {
      add_to_num(formatted_string, " ", writer->flags.minus_flag,
                 info->left_space);
    }
  }
}

void build_format_string(char **formatted_string, WriterFormat *writer,
                         ExtraInfo *info, va_list vars) {
  if (build_base(formatted_string, writer, info, vars) != FAIL) {
    apply_precision(formatted_string, writer, info);
    info->left_space = apply_width(formatted_string, writer);
    apply_flags(formatted_string, writer, info);
  }
}

int s21_sprintf(char *str, const char *format, ...) {
  va_list vars;
  va_start(vars, format);
  int bad_return = -1, null_chars = 0;
  char *start = str;
  int res = 0;

  while (*format != '\0') {
    for (; *format != '%' && *format; ++str, ++format) {
      *str = *format;
      ++res;
    }
    *str = '\0';
    if (*format) {
      WriterFormat writer;
      init_writer(&writer);
      parse_into_writer(&writer, format + 1);
      validate_writer_flags(&writer);
      if (writer.specification != UNKNOWN) {
        format += writer.parsed_length + 1;

        if (writer.width == ASTERISK) {
          writer.width = va_arg(vars, int);
        }
        if (writer.precision == ASTERISK) {
          writer.precision = va_arg(vars, int);
        }

        ExtraInfo info;
        init_extra_info(&info, s21_strlen(start), &bad_return, &null_chars);

        char *formatted_arg = S21_NULL;
        build_format_string(&formatted_arg, &writer, &info, vars);

        s21_size_t arg_size = s21_strlen(formatted_arg);
        if (formatted_arg != S21_NULL) {
          s21_memmove(str, formatted_arg, arg_size + 1);
        }
        if (writer.specification == 'c' && *formatted_arg == '\0') {
          *str = '\0';
          ++str;
          ++res;
        }
        str += arg_size;
        res += (int)arg_size;

        free(formatted_arg);
      }
    }
  }

  *(str + 1) = '\0';

  return res;
}