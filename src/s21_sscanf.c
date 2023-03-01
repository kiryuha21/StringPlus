#include "s21_sscanf.h"

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

extern const char* specifications;
extern const char* lengths;

void init_reader(ReaderFormat* reader) {
  init_lengths(&(reader->length));
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
  // TODO: (?) wrong combinations should be handled
  while (s21_strchr(lengths, src[reader->parsed_length]) != NULL) {
    if (src[reader->parsed_length] == 'L') {
      ++reader->length.L;
    } else if (src[reader->parsed_length] == 'l') {
      ++reader->length.l;
    } else if (src[reader->parsed_length] == 'h') {
      ++reader->length.h;
    }

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
  if (ch >= 'A' && ch <= 'F') {
    return ch - 'A' + 10;
  }
  return ch - 'a' + 10;
}

void sign_check(const char** str, int* width, int* negative) {
  *negative = *str[0] == '-';
  if (*negative || *str[0] == '+') {
    ++(*str);
    --(*width);
  }
}

long long string_to_ll(const char* str, int width, int base) {
  int negative;
  sign_check(&str, &width, &negative);

  if (starts_with(str, "0x") || starts_with(str, "0X")) {
    str += 2;
    width -= 2;
  } else if (starts_with(str, "0")) {
    ++str;
    --width;
  }

  long long res = 0;
  for (int i = width - 1, power = 0; i >= 0 && isxdigit(str[i]); --i, ++power) {
    res += (long long)powl(base, power) * char_to_num(str[i]);
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

long contains_char_in_first(const char* str, char sym, int width) {
  char* temp = calloc(width + 1, sizeof(char));
  s21_strncpy(temp, str, width);
  char* search_res = s21_strchr(temp, sym);
  if (search_res == NULL) {
    return FAIL;
  }
  return search_res - temp;
}

int starts_with_anycase_str(const char* str, const char* substr) {
  size_t len = s21_strlen(substr);
  for (size_t i = 0; i < len; ++i) {
    if (tolower(substr[i]) != tolower(str[i])) {
      return 0;
    }
  }
  return 1;
}

int is_nan_str(const char* str, int width) {
  int is_negative;
  sign_check(&str, &width, &is_negative);

  if (width < 3) {
    return 0;
  }

  if (starts_with_anycase_str(str, "nan")) {
    return is_negative ? -1 : 1;
  }

  return 0;
}

int is_inf_str(const char* str, int width) {
  int is_negative;
  sign_check(&str, &width, &is_negative);

  if (width < 3) {
    return 0;
  }

  if (starts_with_anycase_str(str, "inf")) {
    return is_negative ? -1 : 1;
  }

  return 0;
}

long double parse_float(const char* str, int width) {
  int nan_search = is_nan_str(str, width);
  if (nan_search != 0) {
    return nan_search == 1 ? nanl("") : -nanl("");
  }

  int inf_search = is_inf_str(str, width);
  if (inf_search != 0) {
    return inf_search == 1 ? INFINITY : -INFINITY;
  }

  int point_search = (int)contains_char_in_first(str, '.', width);
  if (point_search == FAIL) {
    return string_to_ll(str, width, 10);
  }
  long double res = string_to_ll(str, point_search, 10);
  width -= point_search;
  str += point_search;

  ++str;  // skip '.'
  --width;
  int is_positive = res > 0;
  long double float_part = 0;
  for (int i = 0; width > 0 && isdigit(*str); ++i, ++str, --width) {
    long double digit = (*str - '0') * powl(0.1, i + 1);
    float_part = is_positive ? float_part + digit : float_part - digit;
  }
  res += float_part;

  if (tolower(*str) != 'e') {
    return res;
  }

  ++str;  // skip 'e' or 'E'
  int positive_power = *str == '+';
  ++str;  // skip '+' or '-'

  width -= 2;
  long long power = string_to_ll(str, width, 10);
  res *= powl(positive_power ? 10 : 0.1, power);

  return res;
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
    if (reader->length.l) {
      wchar_t* dest = va_arg(args, wchar_t*);
      mbstowcs(dest, str, width);
    } else {
      char* dest = va_arg(args, char*);
      s21_strncpy(dest, str, width);
    }
  } else if (reader->specification == 'c') {
    if (reader->length.l) {
      wchar_t* dest = va_arg(args, wchar_t*);
      mbtowc(dest, str, 1);
    } else {
      char* dest = va_arg(args, char*);
      *dest = *str;
    }
    info->source_shift = 1;
  } else if (reader->specification == 'n') {
    int* dest = va_arg(args, int*);
    *dest = info->processed_chars;
  } else if (reader->specification == '%') {
    info->source_shift = 1;
  } else if (s21_strchr("di", reader->specification)) {
    long long* dest = va_arg(args, long long*);
    long long converted;
    if (reader->specification == 'i' && is_base_16(str)) {
      converted = string_to_ll(str, width, 16);
    } else if (reader->specification == 'i' && is_base_8(str)) {
      converted = string_to_ll(str, width, 8);
    } else {
      converted = string_to_ll(str, width, 10);
    }
    long long res = apply_signed_length(&reader->length, converted);
    *dest = res;
  } else if (s21_strchr("ouxX", reader->specification)) {
    unsigned long long* dest = va_arg(args, unsigned long long*);
    unsigned long long converted;
    if (reader->specification == 'u') {
      converted = string_to_ll(str, width, 10);
    } else if (s21_strchr("xX", reader->specification)) {
      converted = string_to_ll(str, width, 16);
    } else {
      converted = string_to_ll(str, width, 8);
    }
    unsigned long long res = apply_unsigned_length(&reader->length, converted);
    *dest = res;
  } else if (reader->specification == 'p') {
    void** dest = va_arg(args, void**);
    *dest = (void*)(string_to_ll(str, width, 16));
  } else if (s21_strchr("eEfgG", reader->specification)) {
    if (reader->length.L) {
      long double* dest = va_arg(args, long double*);
      *dest = parse_float(str, width);
    } else {
      double* dest = va_arg(args, double*);
      *dest = (double)parse_float(str, width);
    }
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
