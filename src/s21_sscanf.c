#include "s21_sscanf.h"

#include "s21_string.h"

extern const char* specifications;
extern const char* lengths;

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
