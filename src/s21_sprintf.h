#ifndef C2_S21_STRINGPLUS_0_S21_SPRINTF_H
#define C2_S21_STRINGPLUS_0_S21_SPRINTF_H

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

void init_writer(WriterFormat* writer);
int parse_into_writer(WriterFormat* writer,
                      const char* src);  // src = pointer to '%'
int validate_writer(WriterFormat* writer);

int s21_sprintf(char* str, const char* format, ...);

#endif  // C2_S21_STRINGPLUS_0_S21_SPRINTF_H
