#ifndef C2_S21_STRINGPLUS_0_S21_SSCANF_H
#define C2_S21_STRINGPLUS_0_S21_SSCANF_H

#include "s21_string.h"

typedef struct AssignmentInfo {
  int return_code;
  int source_shift;
  int processed_chars;
} AssignmentInfo;

void init_assignment_info(AssignmentInfo* info, int code, int shift, int chars);

typedef struct ReaderFormat {
  int skip_assignment;  // true if asterisk before specification
  int width;
  int parsed_length;
  Lengths length;
  char specification;
} ReaderFormat;

void init_reader(ReaderFormat* reader);
void parse_into_reader(ReaderFormat* reader,
                       const char* src);  // src = pointer to '%'

#endif  // C2_S21_STRINGPLUS_0_S21_SSCANF_H
