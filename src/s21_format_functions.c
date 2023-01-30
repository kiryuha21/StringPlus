#include "s21_format_functions.h"

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

#include "s21_string.h"

// int s21_sscanf(const char *str, const char *format, ...);
// int s21_sprintf(char *str, const char *format, ...);

const char* specifications = "cdieEfgGosuxXpn%";
const char* writer_flags = "-+ #0";
const char* lengths = "hlL";
const char* decimal_places = "0123456789ABCDEF";

void init_flags(Flags* flags) {
  flags->lattice_flag = 0;
  flags->minus_flag = 0;
  flags->plus_flag = 0;
  flags->space_flag = 0;
  flags->zero_flag = 0;
}

void init_lengths(Lengths* lens) {
  lens->L = 0;
  lens->l = 0;
  lens->h = 0;
}

void init_writer(WriterFormat* writer) {
  init_flags(&writer->flags);
  init_lengths(&writer->length);
  writer->precision = UNKNOWN;
  writer->width = UNKNOWN;
  writer->specification = UNKNOWN;
  writer->parsed_length = 0;
}

int str_to_int(const char* str, int* index) {
  int res = 0;
  if (!isdigit(*str)) {
    return -1;
  }
  for (; *str && isdigit(*str); ++str, ++(*index)) {
    res = res * 10 + *str - '0';
  }
  return res;
}

int validate_writer_flags(WriterFormat* writer) {
  if (writer->flags.plus_flag > 1) {
    writer->flags.plus_flag = 1;
  }

  if (writer->precision != UNKNOWN && writer->flags.zero_flag) {
    writer->flags.zero_flag = 0;
  }

  if (!s21_strchr("oxXeEfu", writer->specification)) {
    writer->flags.lattice_flag = 0;
  }

  if ((writer->flags.zero_flag || writer->flags.space_flag) &&
      s21_strchr("csp%", writer->specification)) {
    writer->flags.zero_flag = 0;
  }

  if (s21_strchr("oxXcu%", writer->specification)) {
    writer->flags.plus_flag = 0;
    writer->flags.space_flag = 0;
  }

  return OK;
}

int validate_writer_length(WriterFormat* writer) {
  if (writer->specification == UNKNOWN) {
    return FAIL;
  }

  // TODO: diff types
  if (writer->length.h != 0 &&
      s21_strchr("idouxX", writer->specification) == NULL) {
    return FAIL;
  }
  if (writer->length.l != 0 &&
      s21_strchr("idouxX", writer->specification) == NULL) {
    return FAIL;
  }
  if (writer->length.L != 0 &&
      s21_strchr("eEfgG", writer->specification) == NULL) {
    return FAIL;
  }

  return OK;
}

int validate_writer(WriterFormat* writer) {
  if (validate_writer_flags(writer) == FAIL ||
      // validate_writer_length(writer) == FAIL ||
      !s21_strchr(specifications, writer->specification)) {
    return FAIL;
  }

  return OK;
}

void parse_into_writer(WriterFormat* writer, const char* src) {
  // flags
  while (s21_strchr(writer_flags, src[writer->parsed_length]) != NULL) {
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
    } else {
      int precision =
          str_to_int(&src[writer->parsed_length], &writer->parsed_length);
      if (precision != -1) {
        writer->precision = precision;
      }
    }
  }

  // length
  while (s21_strchr(lengths, src[writer->parsed_length]) != NULL) {
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
  if (s21_strchr(specifications, src[writer->parsed_length]) != NULL) {
    writer->specification = src[writer->parsed_length];
    ++writer->parsed_length;
  }
}

void init_reader(ReaderFormat* reader) {
  reader->length = UNKNOWN;
  reader->width = UNKNOWN;
  reader->specification = UNKNOWN;
  reader->skip_assignment = UNKNOWN;
  reader->parsed_length = 0;
}

void check_reader_asterisk(ReaderFormat* reader, const char* src) {
  if (*src == '*') {
    reader->skip_assignment = 1;
    reader->parsed_length += 1;
  }
}

void parse_into_reader(ReaderFormat* reader, const char* src) {
  // width
  check_reader_asterisk(reader, src + reader->parsed_length);
  int width = str_to_int(&src[reader->parsed_length], &reader->parsed_length);
  if (width != -1) {
    reader->width = width;
  }

  // length
  check_reader_asterisk(reader, src + reader->parsed_length);
  if (s21_strchr(lengths, src[reader->parsed_length]) != NULL) {
    reader->length = src[reader->parsed_length];
    ++reader->parsed_length;
  }

  // specification
  check_reader_asterisk(reader, src + reader->parsed_length);
  if (s21_strchr(specifications, src[reader->parsed_length]) != NULL) {
    reader->specification = src[reader->parsed_length];
    ++reader->parsed_length;
  }
}

int get_digits_amount(unsigned long long num, int number_system) {
  if (num == 0) {
    return 1;
  }

  return (int)floorl(log2l((num)) / log2l(number_system)) + 1;
}

void convert_ll_to_string(long long num, int number_system, char** str) {
  int len = get_digits_amount(num, number_system);

  *str = (char*)calloc(sizeof(char), len + 4);

  if (*str != NULL) {
    for (int i = len - 1; i >= 0 && num > 0; --i, num /= number_system) {
      (*str)[i] = (char)(decimal_places[num % number_system]);
    }
  }
}

// int s21_sscanf(const char *str, const char *format, ...) {
//
// }

void safe_replace(char** dst, char** replacer) {
  if (*dst != NULL) {
    free(*dst);
  }
  *dst = *replacer;
}

double custom_round(double num, int precision) {
  return round(num * pow(10, precision)) * pow(0.1, precision);
}

int handle_zero(char** formatted_string, WriterFormat* writer) {
  writer->flags.lattice_flag = 0;
  *formatted_string = (char*)calloc(5, sizeof(char));
  if (*formatted_string == NULL) {
    return FAIL;
  }
  (*formatted_string)[0] = '0';
  return OK;
}

long long handle_overflow(long long int num, WriterFormat* writer) {
  if (num < 0) {
    writer->flags.plus_flag = -1;
  }
  return llabs(num);
}

void apply_decimal_length(WriterFormat* writer, long long num,
                          int number_system, char** formatted_string) {
  if (s21_strchr("id", writer->specification)) {
    if (writer->length.l == 1 || writer->length.L == 1) {
      long li = (long)num;
      long long res = handle_overflow(li, writer);
      convert_ll_to_string(res, number_system, formatted_string);
    } else if (writer->length.l >= 2 || writer->length.L >= 2) {
      long long lli = (long long)num;
      long long res = handle_overflow(lli, writer);
      convert_ll_to_string(res, number_system, formatted_string);
    } else if (writer->length.h == 1) {
      short si = (short)num;
      long long res = handle_overflow(si, writer);
      convert_ll_to_string(res, number_system, formatted_string);
    } else if (writer->length.h >= 2) {
      char ci = (char)num;
      long long res = handle_overflow(ci, writer);
      convert_ll_to_string(res, number_system, formatted_string);
    } else {
      int i = (int)num;
      long long res = handle_overflow(i, writer);
      convert_ll_to_string(res, number_system, formatted_string);
    }
  } else if (s21_strchr("xXuo", writer->specification)) {
    if (writer->length.l == 1 || writer->length.L == 1) {
      unsigned long uli = (unsigned long)num;
      convert_ll_to_string((long long)uli, number_system, formatted_string);
    } else if (writer->length.l >= 2 || writer->length.L >= 2) {
      unsigned long long ulli = (unsigned long long)num;
      convert_ll_to_string((long long)ulli, number_system, formatted_string);
    } else if (writer->length.h == 1) {
      unsigned short usi = (unsigned short)num;
      convert_ll_to_string(usi, number_system, formatted_string);
    } else if (writer->length.h >= 2) {
      unsigned char uci = (unsigned char)num;
      convert_ll_to_string(uci, number_system, formatted_string);
    } else {
      unsigned int i = (unsigned int)num;
      convert_ll_to_string(i, number_system, formatted_string);
    }
  }
}

// const char* specifications = "cdieEfgGosuxXpn%";
// const char* writer_flags = "-+ #0";
// const char* lengths = "hlL";
int build_base(char** formatted_string, WriterFormat* writer, va_list vars) {
  if (s21_strchr("dioxXu", writer->specification)) {
    long long num = va_arg(vars, long long);

    if (num == 0) {
      return handle_zero(formatted_string, writer);
    }

    if (num < 0 && s21_strchr("di", writer->specification)) {
      writer->flags.plus_flag = -1;
    }

    int number_system = 10;
    if (s21_strchr("xX", writer->specification)) {
      number_system = 16;
    } else if (s21_strchr("o", writer->specification)) {
      number_system = 8;
    }

    apply_decimal_length(writer, llabs(num), number_system, formatted_string);

    if (s21_strchr("x", writer->specification)) {
      char* temp = s21_to_lower(*formatted_string);
      safe_replace(formatted_string, &temp);
    }
  } else if (writer->specification == 'c' || writer->specification == '%') {
    int num = '%';
    if (writer->specification == 'c') {
      num = va_arg(vars, int);
    } else {
      writer->width = UNKNOWN;
    }
    writer->specification = 'c';
    *formatted_string = (char*)calloc(sizeof(char), 2);
    if (*formatted_string == NULL ||
        ((writer->length.l || writer->length.L) && !(num >= 0 && num <= 255))) {
      return FAIL;
    }
    (*formatted_string)[0] = (char)num;
  } else if (writer->specification == 'f') {
    double num = va_arg(vars, double);
    int precision =
        writer->precision == UNKNOWN ? DEFAULT_PRECISION : writer->precision;
    num = custom_round(num, precision);
    int len = get_digits_amount((int)num, 10) + 1 + precision;

    double d;
    double float_part = modf(num, &d);
    int decimal_part = abs((int)d);

    *formatted_string = (char*)calloc(sizeof(char), len + 4);
    if (*formatted_string == NULL) {
      return FAIL;
    }

    // TODO: (?) another floating part in original sprintf...
    int decimal_len = get_digits_amount(decimal_part, 10);
    for (int i = decimal_len + 1; precision > 0; ++i, --precision) {
      float_part *= 10;
      (*formatted_string)[i] = (char)((int)float_part % 10 + '0');
      float_part -= (int)float_part;
    }
    (*formatted_string)[decimal_len] = '.';
    for (int i = decimal_len - 1; i >= 0; --i, decimal_part /= 10) {
      (*formatted_string)[i] = (char)(decimal_part % 10 + '0');
    }
  }
  return OK;
}

size_t apply_width(char** formatted_string, WriterFormat* writer) {
  size_t res = 0;
  if (writer->width != UNKNOWN) {
    int len = (int)s21_strlen(*formatted_string);
    if (writer->width > len) {
      size_t diff = writer->width - len;

      char* spacer = (char*)calloc(sizeof(char), diff + 1);
      s21_memset(
          spacer,
          writer->flags.zero_flag && !writer->flags.minus_flag ? '0' : ' ',
          diff);

      char* with_spacer;
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

void apply_precision(char** formatted_string, WriterFormat* writer) {
  if (writer->precision != UNKNOWN) {
    if (s21_strchr("iduoxX", writer->specification)) {
      size_t len = s21_strlen(*formatted_string);
      if (writer->precision >= (int)len) {
        char* trimmed = s21_trim(*formatted_string, " ");
        safe_replace(formatted_string, &trimmed);

        int diff = writer->precision - (int)s21_strlen(*formatted_string);
        char* null_spacer = (char*)calloc(sizeof(char), diff + 1);
        s21_memset(null_spacer, '0', diff);

        char* result = s21_insert(*formatted_string, null_spacer, 0);
        safe_replace(formatted_string, &result);
        free(null_spacer);
      }
    }
  }
}

void add_to_num(char** formatted_string, const char* str, int reverse,
                size_t left_space) {
  if (!reverse) {
    size_t len = s21_strlen(str);
    if (len > left_space) {
      char* null_spaces = (char*)calloc(len - left_space, sizeof(char));
      s21_memset(null_spaces, ' ', len - left_space);
      char* additional_space = s21_insert(*formatted_string, null_spaces, 0);
      safe_replace(formatted_string, &additional_space);
      for (size_t i = 0; i < len; ++i) {
        (*formatted_string)[i] = str[i];
      }
    } else {
      size_t start = 0;
      for (; (*formatted_string)[start + len] == ' '; ++start) {
      }
      for (size_t i = 0; i < len; ++i, ++start) {
        (*formatted_string)[start] = str[i];
      }
    }
  } else {
    size_t len = s21_strlen(str);
    if (len > left_space) {
      char* null_spaces = (char*)calloc(len - left_space, sizeof(char));
      s21_memset(null_spaces, ' ', len - left_space);
      char* additional_space = s21_insert(*formatted_string, null_spaces,
                                          s21_strlen(*formatted_string));
      safe_replace(formatted_string, &additional_space);
    }
    size_t i = 0;
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

void apply_flags(char** formatted_string, WriterFormat* writer,
                 size_t left_space) {
  if (s21_strchr("id", writer->specification)) {
    if (writer->flags.zero_flag) {
      char* str = *formatted_string;
      for (; *str == ' '; ++str) {
        *str = '0';
      }
    }
  }
  if (writer->flags.plus_flag == 1) {
    add_to_num(formatted_string, "+", writer->flags.minus_flag, left_space);
  } else if (writer->flags.plus_flag == -1) {
    add_to_num(formatted_string, "-", writer->flags.minus_flag, left_space);
  } else if (writer->flags.space_flag) {
    add_to_num(formatted_string, " ", writer->flags.minus_flag, left_space);
  }
  if (writer->flags.lattice_flag) {
    // check if 0 is already in the right place
    if (writer->specification == 'o') {
      char* already = s21_strchr(*formatted_string, '0');
      int first = already == NULL ? 0 : 1;
      if (first) {
        for (char* ptr = *formatted_string; ptr != already; ++ptr) {
          if (*ptr != ' ' && *ptr != '0') {
            first = 0;
          }
        }
      }
      // if it's not
      if (!first) {
        add_to_num(formatted_string, "0", writer->flags.minus_flag, left_space);
      }
    } else if (writer->specification == 'x') {
      add_to_num(formatted_string, "0x", writer->flags.minus_flag, left_space);
    } else if (writer->specification == 'X') {
      add_to_num(formatted_string, "0X", writer->flags.minus_flag, left_space);
    }
  }
}

void build_format_string(char** formatted_string, WriterFormat* writer,
                         va_list vars) {
  if (build_base(formatted_string, writer, vars) != FAIL) {
    apply_precision(formatted_string, writer);
    size_t left_space = apply_width(formatted_string, writer);
    apply_flags(formatted_string, writer, left_space);
  }
}

int s21_sprintf(char* str, const char* format, ...) {
  int result = 0;
  va_list vars;
  va_start(vars, format);
  s21_memset(str, '\0', 100);

  while (*format != '\0') {
    for (; *format != '%'; ++str, ++format) {
      *str = *format;
    }
    WriterFormat writer;
    init_writer(&writer);
    parse_into_writer(&writer, format + 1);
    format += writer.parsed_length + 1;
    if (validate_writer(&writer) == OK) {
      ++result;

      if (writer.width == ASTERISK) {
        writer.width = va_arg(vars, int);
      }
      if (writer.precision == ASTERISK) {
        writer.precision = va_arg(vars, int);
      }

      char* formatted_arg = NULL;
      build_format_string(&formatted_arg, &writer, vars);

      size_t size_before = s21_strlen(str);
      s21_strcat(str, formatted_arg);
      str += s21_strlen(str) - size_before;

      free(formatted_arg);
    }
  }
  *(str + 1) = '\0';

  va_end(vars);
  return result;
}