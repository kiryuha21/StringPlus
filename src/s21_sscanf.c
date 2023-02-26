#include "s21_sscanf.h"

#include <stdarg.h>
#include <stdlib.h>

extern const char* specifications;
extern const char* lengths;

void init_reader(ReaderFormat* reader) {
  reader->length = UNKNOWN;
  reader->width = UNKNOWN;
  reader->specification = UNKNOWN;
  reader->skip_assignment = UNKNOWN;
  reader->parsed_length = 0;
}

AssignmentInfo* new_assignment_info() {
    AssignmentInfo* info = calloc(1, sizeof(AssignmentInfo));
    info->return_code = UNKNOWN;
    info->source_shift = UNKNOWN;
    return info;
}

void free_assignment_info(AssignmentInfo* info) {
    if (info != NULL) {
        free(info);
    }
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

int define_width(ReaderFormat* reader, const char* str) {
    if (reader->width != UNKNOWN) {
        return reader->width;
    }
    char* closest_space = s21_strchr(str, ' ');
    if (closest_space == NULL) {
        return (int)(str + s21_strlen(str) - str);
    }
    return (int)(closest_space - str);
}

AssignmentInfo* process_format_string(const char* str, ReaderFormat* reader, va_list args) {
    AssignmentInfo* info = new_assignment_info();

    int width = define_width(reader, str);
    if (reader->specification == 's') {
        char* dest = va_arg(args, char*);
        s21_strncpy(dest, str, width);
    } // else if

    info->return_code = OK;
    info->source_shift = width;
    while (*(str + info->source_shift) == ' ') {
        ++(info->source_shift);
    }
    return info;
}

int s21_sscanf(const char* str, const char* format, ...) {
  va_list args;
  va_start(args, format);

  int return_res = 0;
  int mismatch_found = 0;
  while (*format != '\0' && !mismatch_found) {
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

      AssignmentInfo* info = process_format_string(str, &reader, args);
      str += info->source_shift;
      format += reader.parsed_length + 1;

      return_res += info->return_code;
        free_assignment_info(info);
    }
  }

  va_end(args);
  return 1;
}
