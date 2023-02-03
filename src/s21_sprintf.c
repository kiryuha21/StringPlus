#include "s21_sprintf.h"

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

#include "s21_string.h"

extern const char* specifications;
extern const char* writer_flags;
extern const char* lengths;
extern const char* decimal_places;

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

void validate_writer_flags(WriterFormat* writer) {
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
        s21_strchr("uoid", writer->specification)) {
      writer->flags.zero_flag = 0;
    }

    if (!s21_strchr("oxXeEfu", writer->specification)) {
      writer->flags.lattice_flag = 0;
    }

    // TODO: should be determined
    if ((writer->flags.zero_flag || writer->flags.space_flag) &&
        s21_strchr("cs%", writer->specification)) {
      writer->flags.zero_flag = 0;
    }

    if (s21_strchr("oxXcuo%", writer->specification)) {
      writer->flags.plus_flag = 0;
      writer->flags.space_flag = 0;
    }
  }
}

// TODO: test
int validate_writer(WriterFormat* writer) {
  validate_writer_flags(writer);
  return OK;
}

char* get_writer_flags(WriterFormat* writer) {
  validate_writer(writer);
  char* res = (char*)calloc(6, sizeof(char));
  res[0] = '%';
  int i = 1;
  if (writer->flags.lattice_flag) {
    res[i++] = '#';
  }
  if (writer->flags.plus_flag) {
    res[i++] = '+';
  }
  if (writer->flags.space_flag) {
    res[i++] = ' ';
  }
  if (writer->flags.minus_flag) {
    res[i++] = '-';
  }
  if (writer->flags.zero_flag) {
    res[i++] = '0';
  }
  return res;
}

int parse_into_writer(WriterFormat* writer, const char* src) {
  int res = -1;
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
    res = -2;
    ++writer->parsed_length;
    if (src[writer->parsed_length] == '*') {
      writer->precision = ASTERISK;
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
  while (s21_strchr(lengths, src[writer->parsed_length]) != NULL) {
    if (src[writer->parsed_length] == 'L') {
      if (writer->length.l || writer->length.h || writer->length.L == 1) {
        return writer->parsed_length;
      }
      ++writer->length.L;
    } else if (src[writer->parsed_length] == 'l') {
      if (writer->length.L || writer->length.h || writer->length.l == 2) {
        return writer->parsed_length;
      }
      ++writer->length.l;
    } else if (src[writer->parsed_length] == 'h') {
      if (writer->length.l || writer->length.L || writer->length.h == 2) {
        return writer->parsed_length;
      }
      ++writer->length.h;
    }

    ++writer->parsed_length;
    res = writer->parsed_length;
  }

  // specification
  if (s21_strchr(specifications, src[writer->parsed_length]) != NULL) {
    writer->specification = src[writer->parsed_length];
    ++writer->parsed_length;
  } else {
    return res;
  }

  return 0;
}

int get_digits_amount(unsigned long long num, int number_system) {
  if (num == 0) {
    return 1;
  }

  return (int)floorl(log2l((num)) / log2l(number_system)) + 1;
}

void convert_ll_to_string(ull num, int number_system, char** str) {
  int len = get_digits_amount(num, number_system);

  *str = (char*)calloc(sizeof(char), len + 4);

  if (num == 0) {
    **str = '0';
    return;
  }

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

long long handle_overflow(long long int num, WriterFormat* writer) {
  if (num < 0) {
    writer->flags.plus_flag = -1;
  }
  return llabs(num);
}

void apply_signed_length(WriterFormat* writer, long long num, int number_system,
                         char** formatted_string) {
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
}

void apply_unsigned_length(WriterFormat* writer, unsigned long long num,
                           int number_system, char** formatted_string) {
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
    unsigned int ui = (unsigned int)num;
    convert_ll_to_string(ui, number_system, formatted_string);
  }
}

// const char* specifications = "cdieEfgGosuxXpn%";
// const char* writer_flags = "-+ #0";
// const char* lengths = "hlL";
int build_base(char** formatted_string, WriterFormat* writer,
               size_t written_bytes, int* bad_return, int* null_chars,
               va_list vars) {
  if (s21_strchr("di", writer->specification)) {
    long long num = va_arg(vars, long long);

    apply_signed_length(writer, num, 10, formatted_string);

  } else if (s21_strchr("oxXu", writer->specification)) {
    unsigned long long num = va_arg(vars, unsigned long long);

    int number_system = 10;
    if (s21_strchr("xX", writer->specification)) {
      number_system = 16;
    } else if (s21_strchr("o", writer->specification)) {
      number_system = 8;
    }

    apply_unsigned_length(writer, num, number_system, formatted_string);

    if (s21_strchr("x", writer->specification)) {
      char* temp = s21_to_lower(*formatted_string);
      safe_replace(formatted_string, &temp);
    }

    if (**formatted_string == '0' && s21_strchr("xX", writer->specification)) {
      writer->flags.lattice_flag = 0;
      *bad_return = 0;
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
    if (num == '\0') {
      (*null_chars) +=
          (writer->flags.minus_flag && writer->width != UNKNOWN ? writer->width
                                                                : 1);
      writer->width = (writer->width == UNKNOWN || writer->flags.minus_flag
                           ? UNKNOWN
                           : writer->width - 1);
    }
    (*formatted_string)[0] = (char)num;
  } else if (writer->specification == 'f') {
    double num = va_arg(vars, double);
    int precision = writer->precision == UNKNOWN || writer->precision == EMPTY
                        ? DEFAULT_PRECISION
                        : writer->precision;
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
  } else if (writer->specification == 's') {
    char* string = va_arg(vars, char*);
    *formatted_string = (char*)calloc(s21_strlen(string) + 1, sizeof(char));
    s21_strcpy(*formatted_string, string);
  } else if (writer->specification == 'p') {
    writer->length.l = 1;
    writer->flags.lattice_flag = 1;

    void* pointer = va_arg(vars, void*);
    if (pointer == NULL) {
      *formatted_string = (char*)calloc(6, sizeof(char));
      char* temp = s21_insert(*formatted_string, "(nil)", 0);
      safe_replace(formatted_string, &temp);
      writer->flags.lattice_flag = 0;
      writer->flags.plus_flag = 0;
      writer->flags.zero_flag = 0;
      writer->flags.space_flag = 0;
      writer->precision = UNKNOWN;
      return OK;
    }
    apply_unsigned_length(writer, (ull)pointer, 16, formatted_string);

    char* lowered = s21_to_lower(*formatted_string);
    safe_replace(formatted_string, &lowered);
  } else if (writer->specification == 'n') {
    int* num = va_arg(vars, int*);
    *num = (int)written_bytes;
    *bad_return = 0;
  }
  return OK;
}

size_t apply_width(char** formatted_string, WriterFormat* writer) {
  size_t res = 0;
  if (writer->width != UNKNOWN && writer->specification != 'n') {
    int len = (int)s21_strlen(*formatted_string);
    if (writer->width > len) {
      size_t diff = writer->width - len;

      char* spacer = (char*)calloc(sizeof(char), diff + 1);
      s21_memset(spacer, ' ', diff);

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

void insert_null_spacer_at_index(char** src, int index, int nsp_size) {
  char* null_spacer = (char*)calloc(sizeof(char), nsp_size + 1);
  s21_memset(null_spacer, '0', nsp_size);

  char* result = s21_insert(*src, null_spacer, index);
  safe_replace(src, &result);
  free(null_spacer);
}

void apply_precision(char** formatted_string, WriterFormat* writer,
                     int* bad_return) {
  if (writer->precision != UNKNOWN) {
    if (s21_strchr("iduoxX", writer->specification)) {
      if ((writer->precision != EMPTY && writer->precision != 0) ||
          **formatted_string != '0') {
        size_t len = s21_strlen(*formatted_string);
        if (writer->precision >= (int)len) {
          char* trimmed = s21_trim(*formatted_string, " ");
          safe_replace(formatted_string, &trimmed);

          int diff = writer->precision - (int)s21_strlen(*formatted_string);
          insert_null_spacer_at_index(formatted_string, 0, diff);
        }
      } else {
        **formatted_string = '\0';
        *bad_return = 0;
      }
    } else if (writer->specification == 's') {  // TODO: or maybe smth else too
      if (writer->precision < (int)s21_strlen(*formatted_string)) {
        char* cutted = (char*)calloc(writer->precision + 1, sizeof(char));
        s21_strncpy(cutted, *formatted_string, writer->precision);
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
  if (s21_strchr("uid", writer->specification)) {
    if (writer->flags.zero_flag) {
      char* str = *formatted_string;
      for (; *str == ' '; ++str) {
        *str = '0';
      }
    }
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
    } else if (writer->specification == 'x' || writer->specification == 'p') {
      if (writer->flags.plus_flag == 1) {
        add_to_num(formatted_string, "+0x", writer->flags.minus_flag,
                   left_space);
      } else if (writer->flags.space_flag) {
        add_to_num(formatted_string, " 0x", writer->flags.minus_flag,
                   left_space);
      } else {
        add_to_num(formatted_string, "0x", writer->flags.minus_flag,
                   left_space);
      }
    } else if (writer->specification == 'X') {
      add_to_num(formatted_string, "0X", writer->flags.minus_flag, left_space);
    }
  }

  // TODO: check for specifications inapplicable with these flags
  if (writer->specification != 'n' && writer->specification != 'p') {
    if (writer->flags.plus_flag == 1) {
      add_to_num(formatted_string, "+", writer->flags.minus_flag, left_space);
    } else if (writer->flags.plus_flag == -1) {
      add_to_num(formatted_string, "-", writer->flags.minus_flag, left_space);
    } else if (writer->flags.space_flag) {
      add_to_num(formatted_string, " ", writer->flags.minus_flag, left_space);
    }
  }
}

void build_format_string(char** formatted_string, WriterFormat* writer,
                         size_t written_bytes, int* bad_return, int* null_chars,
                         va_list vars) {
  if (build_base(formatted_string, writer, written_bytes, bad_return,
                 null_chars, vars) != FAIL) {
    apply_precision(formatted_string, writer, bad_return);
    size_t left_space = apply_width(formatted_string, writer);
    apply_flags(formatted_string, writer, left_space);
  }
}

int s21_sprintf(char* str, const char* format, ...) {
  va_list vars;
  va_start(vars, format);
  s21_memset(str, '\0', 100);  // TODO: hey wtf this should be removed
  int bad_return = -1, null_chars = 0;
  char* start = str;

  while (*format != '\0') {
    for (; *format != '%' && *format; ++str, ++format) {
      *str = *format;
    }
    if (*format) {
      WriterFormat writer;
      init_writer(&writer);
      int skip = parse_into_writer(&writer, format + 1);
      validate_writer(&writer);
      if (skip == 0 && writer.specification != UNKNOWN) {
        format += writer.parsed_length + 1;

        if (writer.width == ASTERISK) {
          writer.width = va_arg(vars, int);
        }
        if (writer.precision == ASTERISK) {
          writer.precision = va_arg(vars, int);
        }

        char* formatted_arg = NULL;
        build_format_string(&formatted_arg, &writer, s21_strlen(start),
                            &bad_return, &null_chars, vars);

        size_t size_before = s21_strlen(str);
        s21_strcat(str, formatted_arg);
        str += s21_strlen(str) - size_before;

        free(formatted_arg);
      } else if (!*(format + 1)) {
        *str = '%';
        ++str;
        ++format;
      } else {
        const char* char_to_skip = format + (skip >= 0 ? skip : 0);
        char* add_flags = get_writer_flags(&writer);
        for (char* ptr = add_flags; *ptr; ++ptr, ++str) {
          *str = *ptr;
        }
        free(add_flags);
        ++format;                                            // skip %
        for (; s21_strchr(writer_flags, *format); ++format)  // skip flags
          ;
        for (; *format != *char_to_skip; ++str, ++format) {  // copy until skip
          *str = *format;
          if (*str == '.') {
            for (; *(format + 1) == '0'; ++format)
              ;
            if ((*(format + 1) < '0' || *(format + 1) > '9') && skip != -1) {
              ++str;
              *str = '0';
            }
          }
        }
        if (skip != -1) {
          ++format;  // skip lengths
          if (*format == *(format - 1) && *format != 'L') {
            ++format;
          }
        }
      }
    }
  }

  *(str + 1) = '\0';

  va_end(vars);
  // TODO: orig sprintf returns 0 with raw %n, our func returns -1
  return (int)s21_strlen(start) + null_chars
             ? (int)s21_strlen(start) + null_chars
             : bad_return;
}