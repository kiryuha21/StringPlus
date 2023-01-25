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
  Flags flags = writer->flags;
  if (flags.space_flag > 1 || flags.plus_flag > 1 || flags.zero_flag > 1 ||
      flags.minus_flag > 1 || flags.lattice_flag > 1) {
    return FAIL;
  }

  if (flags.space_flag &&
      (flags.plus_flag || s21_strchr("cosp%", writer->specification))) {
    return FAIL;
  }

  if (flags.minus_flag && flags.zero_flag) {
    return FAIL;
  }

  if (flags.lattice_flag && s21_strchr("oeExXfgG", writer->specification)) {
    return FAIL;
  }

  return OK;
}

int validate_writer_length(WriterFormat* writer) {
  if (writer->specification == UNKNOWN) {
    return FAIL;
  }

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
      validate_writer_length(writer) == FAIL) {
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

int get_digits_amount(int num) {
  if (num == 0) {
    return 1;
  }

  int res = 0;
  if (num < 0) {
    ++res;
  }

  res += (int)floor(log10(abs(num))) + 1;

  return res;
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

// const char* specifications = "cdieEfgGosuxXpn%";
// const char* writer_flags = "-+ #0";
// const char* lengths = "hlL";
int build_base(char** formatted_string, WriterFormat* writer, va_list vars) {
  if (writer->specification == 'd' || writer->specification == 'i') {
    int num = va_arg(vars, int);
    int len = get_digits_amount(num);

    *formatted_string = (char*)calloc(sizeof(char), len + 1);
    if (*formatted_string == NULL) {
      return FAIL;
    }

    if (num < 0) {
      (*formatted_string)[0] = '-';
    }

    num = abs(num);
    for (int i = len - 1; i >= 0 && num > 0; --i, num /= 10) {
      (*formatted_string)[i] = (char)(num % 10 + '0');
    }
  } else if (writer->specification == 'c') {
    int num = va_arg(vars, int);
    *formatted_string = (char*)calloc(sizeof(char), 2);
    if (*formatted_string == NULL) {
      return FAIL;
    }
    (*formatted_string)[0] = (char)num;
  } else if (writer->specification == 'f') {
    double num = va_arg(vars, double);
    int precision =
        writer->precision == UNKNOWN ? DEFAULT_PRECISION : writer->precision;
    int len = get_digits_amount((int)num) + 1 + precision;

    double d;
    double f = modf(num, &d);
    int decimal_part = abs((int)d);
    int float_part = abs((int)(pow(10, precision) * f));

    *formatted_string = (char*)calloc(sizeof(char), len + 1);
    if (*formatted_string == NULL) {
      return FAIL;
    }

    if (num < 0) {
      (*formatted_string)[0] = '-';
    }

    int i;
    for (i = len - 1; precision > 0; --i, --precision, float_part /= 10) {
      if (float_part > 0) {
        (*formatted_string)[i] = (char)(float_part % 10 + '0');
      } else {
        (*formatted_string)[i] = '0';
      }
    }
    (*formatted_string)[i] = '.';
    --i;
    for (; decimal_part > 0 && i >= 0; --i, decimal_part /= 10) {
      (*formatted_string)[i] = (char)(decimal_part % 10 + '0');
    }
  }  // else if other formats...
  return OK;
}

void apply_width(char** formatted_string, int width) {
  if (width != UNKNOWN) {
    int len = (int)s21_strlen(*formatted_string);
    if (width > len) {
      size_t diff = width - len;
      char* spacer = (char*)calloc(sizeof(char), diff + 1);
      s21_memset(spacer, ' ', diff);
      char* with_spacer = s21_insert(*formatted_string, spacer, 0);
      safe_replace(formatted_string, &with_spacer);
      free(spacer);
    }
  }
}

void build_format_string(char** formatted_string, WriterFormat* writer,
                         va_list vars) {
  build_base(formatted_string, writer, vars);
  apply_width(formatted_string, writer->width);
  // apply_precision()
  // apply_flags()
  // ...
}

int s21_sprintf(char* str, const char* format, ...) {
  int result = 0;
  va_list vars;
  va_start(vars, format);

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
