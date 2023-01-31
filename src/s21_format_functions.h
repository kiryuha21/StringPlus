#ifndef C2_S21_STRINGPLUS_0_S21_FORMAT_FUNCTIONS_H
#define C2_S21_STRINGPLUS_0_S21_FORMAT_FUNCTIONS_H

#define EMPTY (-30)
#define ASTERISK (-20)
#define UNKNOWN (-10)
#define FAIL (-1)
#define OK 0

#define DEFAULT_PRECISION 6

typedef unsigned long long ull;

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

typedef struct ReaderFormat {
  int skip_assignment;  // true if asterisk before specification
  int width;
  int parsed_length;
  char length;
  char specification;
} ReaderFormat;

void init_reader(ReaderFormat* reader);
void parse_into_reader(ReaderFormat* reader,
                       const char* src);  // src = pointer to '%'

// s21_format_functions.c:
int s21_sscanf(const char* str, const char* format, ...);
int s21_sprintf(char* str, const char* format, ...);

#endif  // C2_S21_STRINGPLUS_0_S21_FORMAT_FUNCTIONS_H
