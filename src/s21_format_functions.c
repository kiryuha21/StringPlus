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

int validate_writer_flags(WriterFormat* writer) {
  Flags flags = writer->flags;
  if (flags.lattice_flag > 1 || flags.minus_flag > 1 || flags.plus_flag > 1 ||
      flags.zero_flag > 1 || flags.space_flag > 1) {
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

int get_digits_num(int num) {
  if (num == 0) {
    return 1;
  }

  int res = 0;
  if (num < 0) {
    ++res;
  }

  res = (int)floor(log10(abs(num))) + 1;

  return res;
}

int max(int first, int second, int third) {
  if (first > second) {
    return first > third ? first : third;
  }
  return second > third ? second : third;
}

void handle_int(char** str, int var, WriterFormat* writer) {
  int len = max(writer->precision, writer->width, get_digits_num(var));
  *str = (char*)calloc(sizeof(char), len + 1 + 2);  // 2 for flags
  if (str == NULL) {
    return;
  }
  len = get_digits_num(var);
  int offset = writer->width != UNKNOWN ? writer->width : 0;
  if (offset == 0 &&
      (writer->flags.space_flag || writer->flags.plus_flag || var < 0)) {
    offset = 1;
  }
  int cp_var = abs(var);
  for (int i = len - 1 + offset; i >= offset; --i) {
    (*str)[i] = cp_var % 10 + '0';
    cp_var /= 10;
  }
  if (writer->flags.space_flag) {
    (*str)[0] = ' ';
  }
  if (writer->flags.plus_flag && var >= 0) {
    (*str)[0] = '+';
  }
  if (var < 0) {
    (*str)[0] = '-';
  }
}

int handle_va_arg(char** formatted_arg, WriterFormat* writer, va_list vars) {
  if (s21_strchr("id", writer->specification)) {
    int num = va_arg(vars, int);
    *formatted_arg = NULL;
    handle_int(formatted_arg, num, writer);
  }
  return 0;
}

// int s21_sscanf(const char *str, const char *format, ...) {
//
// }

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

    char* formatted_arg = NULL;
    result += handle_va_arg(&formatted_arg, &writer, vars);

    size_t size_before = s21_strlen(str);
    s21_strcat(str, formatted_arg);
    str += s21_strlen(str) - size_before;

    free(formatted_arg);

    format += writer.parsed_length + 1;
  }
  *(str + 1) = '\0';

  va_end(vars);
  return result;
}
