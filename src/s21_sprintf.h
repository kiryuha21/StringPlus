#ifndef C2_S21_STRINGPLUS_0_S21_SPRINTF_H
#define C2_S21_STRINGPLUS_0_S21_SPRINTF_H

#include "s21_string.h"

typedef struct Flags {
  int minus_flag;
  int plus_flag;
  int space_flag;
  int zero_flag;
  int lattice_flag;
} Flags;

typedef struct Lengths {
  int l;
  int L;
  int h;
} Lengths;

typedef struct WriterFormat {
  Flags flags;
  Lengths length;
  char specification;
  int width;      // could be ASTERISK
  int precision;  // could be ASTERISK
  int parsed_length;
} WriterFormat;

typedef struct ExtraInfo {
  size_t written_bytes;
  size_t left_space;
  int* bad_return;
  int* null_chars;
  int flags_applicable;
} ExtraInfo;

void init_extra_info(ExtraInfo* info, size_t written_bytes, int* bad_return,
                     int* null_chars);

void init_writer(WriterFormat* writer);
int parse_into_writer(WriterFormat* writer,
                      const char* src);  // src = pointer to '%'
int validate_writer(WriterFormat* writer);

int s21_sprintf(char* str, const char* format, ...);

#endif  // C2_S21_STRINGPLUS_0_S21_SPRINTF_H
