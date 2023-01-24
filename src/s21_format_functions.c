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
}

void parse_into_writer(WriterFormat* writer, const char* src) {
  int index = 0;

  // flags
  while (s21_strchr(writer_flags, src[index]) != NULL) {
    char buff[2];
    buff[0] = src[index];
    buff[1] = '\0';

    push_back(&writer->flags, buff);

    ++index;
  }

  // width
  if (src[index] == '*') {
    writer->width = ASTERISK;
    ++index;
  } else {
    int width = 0;
    while (isdigit(src[index])) {
      // TODO: read digits and then convert to int and assign to width
      ++index;
    }
    // assignment to width here
    if (width != 0) {
      writer->width = width;
    }
  }

  // precision
  if (src[index] == '.') {
    ++index;
    if (src[index] == '*') {
      writer->precision = ASTERISK;
    } else {
      // TODO: another while with reading chars, then converting to int and
      // assigning to precision
    }
  }

  // length
  if (s21_strchr(lengths, src[index]) != NULL) {
    writer->length = src[index];
    ++index;
  }

  // specification
  if (s21_strchr(specifications, src[index]) != NULL) {
    writer->specification = src[index];
    ++index;
  }

  // TODO: length should be validated after acknowledging the specification
}

void init_reader(ReaderFormat* reader) {
  reader->length = UNKNOWN;
  reader->width = UNKNOWN;
  reader->specification = UNKNOWN;
  reader->skip_assignment = UNKNOWN;
}

void parse_into_reader(ReaderFormat * reader, const char* src) {
    // TODO: should be implemented with the same logic as parse_into_writer()
}

// TODO: vargs or smth like that should be used in these functions
// TODO: it would be nice to know it
// int s21_sscanf(const char *str, const char *format, ...) {
//
// }
//
// int s21_sprintf(char *str, const char *format, ...) {
//
// }
