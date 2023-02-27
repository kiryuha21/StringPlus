#include "s21_sscanf.h"

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

extern const char* specifications;
extern const char* lengths;
extern const char* decimal_places;

void init_reader(ReaderFormat* reader) {
  reader->length = UNKNOWN;
  reader->width = UNKNOWN;
  reader->specification = UNKNOWN;
  reader->skip_assignment = UNKNOWN;
  reader->parsed_length = 0;
}

void init_assignment_info(AssignmentInfo* info, int code, int shift,
                          int chars) {
  info->return_code = code;
  info->source_shift = shift;
  info->processed_chars = chars;
}

void check_reader_asterisk(ReaderFormat* reader, const char* src) {
  if (*src == '*') {
    reader->skip_assignment = 1;
    reader->parsed_length += 1;
  }
}

void parse_into_reader(ReaderFormat* reader, const char* src) {
  // skip assignment check
  check_reader_asterisk(reader, src + reader->parsed_length);

  // width
  int width = str_to_int(&src[reader->parsed_length], &reader->parsed_length);
  if (width != -1) {
    reader->width = width;
  }

  // length
  if (s21_strchr(lengths, src[reader->parsed_length]) != NULL) {
    reader->length = src[reader->parsed_length];
    ++reader->parsed_length;
  }

  // specification
  if (s21_strchr(specifications, src[reader->parsed_length]) != NULL) {
    reader->specification = src[reader->parsed_length];
    ++reader->parsed_length;
  }
}

// width of the word to be scanned
int define_width(ReaderFormat* reader, const char* str) {
  if (reader->width != UNKNOWN) {
    return reader->width;
  }
  char* closest_space = s21_strchr(str, ' ');
  if (closest_space == NULL) {
    return (int)(s21_strlen(str));
  }
  return (int)(closest_space - str);
}

int dist_to_non_space(const char* str) {
  int res = 0;
  for (; *str == ' '; ++str, ++res)
    ;
  return res;
}

int starts_with(const char* str, char* prefix) {
  for (; *prefix != '\0'; ++str, ++prefix) {
    if (*str != *prefix) {
      return 0;
    }
  }
  return 1;
}

int char_to_num(char ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  }
  return ch - 'A' + 10;
}

int string_to_int(const char* str, int width, int base) {
  int negative = str[0] == '-';
  if (negative) {
    ++str;
    --width;
  }

  if (starts_with(str, "0x") || starts_with(str, "0X")) {
    str += 2;
    width -= 2;
  } else if (starts_with(str, "0")) {
    ++str;
    --width;
  }

  int res = 0;
  for (int i = width - 1, power = 0; i >= 0; --i, ++power) {
    res += (int)pow(base, power) * char_to_num(str[i]);
  }
  if (negative) {
    res = -res;
  }

  return res;
}

int is_base_16(const char* str) {
    if (*str == '-') {
        ++str;
    }
    return starts_with(str, "0x") || starts_with(str, "0X");
}

int is_base_8(const char* str) {
    if (*str == '-') {
        ++str;
    }
    return starts_with(str, "0");
}

void process_format_string(const char* str, ReaderFormat* reader,
                           AssignmentInfo* info, va_list args) {
  // default values to be possibly changed for certain specs
  int spaces_until_data = dist_to_non_space(str);
  if (s21_strchr("cn", reader->specification) == NULL) {
    info->source_shift = spaces_until_data;
    str += spaces_until_data;
  }

  int width = define_width(reader, str);
  info->source_shift += width;
  info->return_code = 1;

  if (reader->skip_assignment == 1) {
    return;
  }

  if (reader->specification == 's') {
    char* dest = va_arg(args, char*);
    s21_strncpy(dest, str, width);
  } else if (reader->specification == 'c') {
    char* dest = va_arg(args, char*);
    *dest = *str;
    info->source_shift = 1;
  } else if (reader->specification == 'n') {
    int* dest = va_arg(args, int*);
    *dest = info->processed_chars;
  } else if (reader->specification == '%') {
    info->source_shift = 1;
  } else if (s21_strchr("di", reader->specification)) {
    int* dest = va_arg(args, int*);
    if (reader->specification == 'i' && is_base_16(str)) {
      *dest = string_to_int(str, width, 16);
    } else if (reader->specification == 'i' && is_base_8(str)) {
      *dest = string_to_int(str, width, 8);
    } else {
      *dest = string_to_int(str, width, 10);
    }
  } else if (s21_strchr("ouxX", reader->specification)) {
    unsigned int* dest = va_arg(args, unsigned int*);
    if (reader->specification == 'u') {
      *dest = string_to_int(str, width, 10);
    } else if (s21_strchr("xX", reader->specification)) {
      *dest = string_to_int(str, width, 16);
    } else {
      *dest = string_to_int(str, width, 8);
    }
  } else if (reader->specification == 'p') {
    void** dest = va_arg(args, void**);
    *((unsigned int*)(*dest)) = string_to_int(str, width, 16);
  }
}

int s21_sscanf(const char* str, const char* format, ...) {
  va_list args;
  va_start(args, format);

  int return_res = 0;
  int mismatch_found = 0;
  for (int processed_chars = 0; *format != '\0' && !mismatch_found;
       ++processed_chars) {
    if (*format != '%') {
      if (*format == *str) {
        ++str;
        ++format;
      } else {
        mismatch_found = 1;
      }
    } else {
      ReaderFormat reader;
      init_reader(&reader);
      parse_into_reader(&reader, format + 1);

      AssignmentInfo info;
      init_assignment_info(&info, UNKNOWN, UNKNOWN, processed_chars);

      process_format_string(str, &reader, &info, args);

      str += info.source_shift;
      format += reader.parsed_length + 1;
      return_res += info.return_code;
    }
  }

  va_end(args);
  return return_res;
}
