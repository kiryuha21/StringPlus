#include <ctype.h>
#include <stdlib.h>

#include "s21_string.h"

// int s21_sscanf(const char *str, const char *format, ...);
// int s21_sprintf(char *str, const char *format, ...);

void allocate_and_copy(char** dst, char** src) {
  *dst = (char*)calloc(sizeof(char), s21_strlen(*src) + 1);
  s21_strcpy(*dst, *src);
}

void init_vector(StringVector* vec) {
  vec->size = 0;
  vec->strings = NULL;
}

void push_back(StringVector* vec, char* string) {
  vec->size += 1;
  vec->strings = realloc(vec->strings, sizeof(char*) * (vec->size));
  allocate_and_copy(&vec->strings[vec->size - 1], &string);
}

void clear(StringVector* vec) {
  if (vec->strings != NULL) {
    for (int i = 0; i < vec->size; ++i) {
      free(vec->strings[i]);
    }
    free(vec->strings);
  }
}

void init_writer(WriterFormat* writer) {
  init_vector(&writer->flags);
  writer->length = UNKNOWN;
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
  if (writer->length == UNKNOWN) {
    return OK;
  }

  if (s21_strchr("hlL", writer->length) == NULL) {
    return FAIL;
  }

  if (writer->length == 'h' &&
      s21_strchr("idouxX", writer->specification) == NULL) {
    return FAIL;
  }
  if (writer->length == 'l' &&
      s21_strchr("idouxX", writer->specification) == NULL) {
    return FAIL;
  }
  if (writer->length == 'L' &&
      s21_strchr("eEfgG", writer->specification) == NULL) {
    return FAIL;
  }

  return OK;
}

void parse_into_writer(WriterFormat* writer, const char* src) {
  // flags
  while (s21_strchr(writer_flags, src[writer->parsed_length]) != NULL) {
    char buff[2];
    buff[0] = src[writer->parsed_length];
    buff[1] = '\0';

    push_back(&writer->flags, buff);

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
  if (s21_strchr(lengths, src[writer->parsed_length]) != NULL) {
    writer->length = src[writer->parsed_length];
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

// void parse_into_reader(ReaderFormat* reader, const char* src) {
//   // TODO: should be implemented with the same logic as parse_into_writer()
// }

// TODO: vargs or smth like that should be used in these functions
// TODO: it would be nice to know it
// int s21_sscanf(const char *str, const char *format, ...) {
//
// }

int s21_sprintf(char* str, const char* format, ...) {
  int result = 0;

  while (*format != '\0') {
    for (; *format != '%'; ++str, ++format) {
      *str = *format;
    }
    WriterFormat writer;
    init_writer(&writer);
    parse_into_writer(&writer, format + 1);
    // ...
    format += writer.parsed_length + 1;
  }

  return result;
}
