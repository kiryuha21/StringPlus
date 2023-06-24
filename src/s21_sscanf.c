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
  while (s21_strchr(lengths, src[reader->parsed_length]) != S21_NULL) {
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
  if (s21_strchr(specifications, src[reader->parsed_length]) != S21_NULL) {
    reader->specification = src[reader->parsed_length];
    ++reader->parsed_length;
  }
}

int is_blank(const char* str) {
  s21_size_t len = s21_strlen(str);
  for (s21_size_t i = 0; i < len; ++i) {
    if (!isspace(str[i])) {
      return 0;
    }
  }
  return 1;
}

// width of the word to be scanned
int define_width(ReaderFormat* reader, const char* str) {
  int len = (int)s21_strlen(str);
  char* closest_space = s21_strchr(str, ' ');
  if (closest_space == S21_NULL) {
    if (reader->width == UNKNOWN) {
      return len;
    }
    if (reader->width <= len) {
      return reader->width;
    }
    return len;
  }

  int dist_to_space = (int)(closest_space - str);
  if (reader->width == UNKNOWN) {
    return dist_to_space;
  }
  if (reader->width <= dist_to_space) {
    return reader->width;
  }
  return dist_to_space;
}

int dist_to_non_space(const char* str) {
  int res = 0;
  for (int i = 0; isspace(str[i]); ++i, ++res)
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
  if (ch >= 'a' && ch <= 'f') {
    return ch - 'a' + 10;
  }
  return 0;
}

void sign_check(const char** str, int* width, int* negative) {
  *negative = *str[0] == '-';
  if (*negative || *str[0] == '+') {
    ++(*str);
    --(*width);
  }
}

ull string_to_ull(const char* str, int width, int base, int* negative) {
  sign_check(&str, &width, negative);

  if (starts_with(str, "0x") || starts_with(str, "0X")) {
    str += 2;
    width -= 2;
  } else if (starts_with(str, "0")) {
    ++str;
    --width;
  }

  unsigned long long res = 0;
  unsigned long long mul_base = 1;

  int last_valid_pos = 0;
  for (int i = 0; isxdigit(str[i]) && i < width; ++i, ++last_valid_pos)
    ;

  for (int i = last_valid_pos - 1; i >= 0; --i, mul_base *= base) {
    res += mul_base * char_to_num(str[i]);
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
  if (search_res == S21_NULL) {
    free(temp);
    return FAIL;
  }
  long res = search_res - temp;
  free(temp);
  return res;
}

int starts_with_anycase_str(const char* str, const char* substr) {
  s21_size_t len = s21_strlen(substr);
  for (s21_size_t i = 0; i < len; ++i) {
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
  int is_negative;
  sign_check(&str, &width, &is_negative);

  int nan_search = is_nan_str(str, width);
  if (nan_search != 0) {
    return is_negative ? -NAN : NAN;
  }

  int inf_search = is_inf_str(str, width);
  if (inf_search != 0) {
    return is_negative ? -INFINITY : INFINITY;
  }

  int pass;
  int point_search = (int)contains_char_in_first(str, '.', width);
  if (point_search == FAIL && s21_strpbrk(str, "Ee") == S21_NULL) {
    long long signed_res = (long long)string_to_ull(str, width, 10, &pass);
    return signed_res * (is_negative ? -1 : 1);
  }

  long double res;
  if (point_search != FAIL) {
    res = string_to_ull(str, point_search, 10, &pass);
    width -= point_search;
    str += point_search;

    ++str;  // skip '.'
    --width;
    long double float_part = 0;
    for (int i = 0; width > 0 && isdigit(*str); ++i, ++str, --width) {
      long double digit = (*str - '0') * powl(0.1, i + 1);
      float_part += digit;
    }
    res += float_part;
  } else {
    int num_width = (int)(s21_strpbrk(str, "eE") - str);
    res = string_to_ull(str, num_width, 10, &pass);
    width -= num_width;
    str += num_width;
  }

  if (tolower(*str) != 'e') {
    return res * (is_negative ? -1 : 1);
  }

  ++str;  // skip 'e' or 'E'
  int positive_power = *str == '+';
  ++str;  // skip '+' or '-'

  width -= 2;
  long long power = (long long)string_to_ull(str, width, 10, &pass);
  res *= powl(positive_power ? 10 : 0.1, power);

  return res * (is_negative ? -1 : 1);
}

void process_format_string(const char* str, ReaderFormat* reader,
                           AssignmentInfo* info, va_list args) {
  // default values to be possibly changed for certain specs
  int spaces_until_data = dist_to_non_space(str);
  if (s21_strchr("cn", reader->specification) == S21_NULL) {
    info->source_shift = spaces_until_data;
    str += spaces_until_data;
  }

  int width = define_width(reader, str);
  info->source_shift += width;
  info->return_code = 1;

  if (reader->skip_assignment == 1) {
    info->return_code = 0;
    return;
  }

  if (reader->specification == 's') {
    if (reader->length.l) {
      wchar_t* dest = va_arg(args, wchar_t*);
      mbstowcs(dest, str, width);
    } else {
      if (*str == '\0') {
        info->return_code = 0;
      } else {
        char* dest = va_arg(args, char*);
        s21_strncpy(dest, str, width);
        dest[width] = '\0';
      }
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
    long long* dest = va_arg(args, long long*);
    assign_signed_casted_deref(dest, info->processed_chars, &reader->length);
    info->return_code = 0;
    info->source_shift = 0;
  } else if (reader->specification == '%') {
    info->source_shift = spaces_until_data + 1;
    info->return_code = 0;
  } else if (s21_strchr("di", reader->specification)) {
    long long* dest = va_arg(args, long long*);
    long long converted;
    int is_negative;
    if (reader->specification == 'i' && is_base_16(str)) {
      converted = (long long)string_to_ull(str, width, 16, &is_negative);
    } else if (reader->specification == 'i' && is_base_8(str)) {
      converted = (long long)string_to_ull(str, width, 8, &is_negative);
    } else {
      converted = (long long)string_to_ull(str, width, 10, &is_negative);
    }
    converted *= (is_negative ? -1 : 1);
    long long res = apply_signed_length(&reader->length, converted);
    assign_signed_casted_deref(dest, res, &reader->length);
  } else if (s21_strchr("ouxX", reader->specification)) {
    unsigned long long* dest = va_arg(args, unsigned long long*);
    unsigned long long converted;
    int is_negative;
    if (reader->specification == 'u') {
      converted = string_to_ull(str, width, 10, &is_negative);
    } else if (s21_strchr("xX", reader->specification)) {
      converted = string_to_ull(str, width, 16, &is_negative);
    } else {
      converted = string_to_ull(str, width, 8, &is_negative);
    }
    converted *= (is_negative ? -1 : 1);
    unsigned long long res = apply_unsigned_length(&reader->length, converted);
    assign_unsigned_casted_deref(dest, res, &reader->length);
  } else if (reader->specification == 'p') {
    int pass;
    void** dest = va_arg(args, void**);
#if defined(__linux__)
    if (starts_with_anycase_str(str, "(nil)") && width >= 5) {
      *dest = S21_NULL;
    }
#elif __APPLE__
    if (starts_with_anycase_str(str, "0x0") && width >= 3) {
      *dest = S21_NULL;
    }
#endif
    else {
      *dest = (void*)(string_to_ull(str, width, 16, &pass));
    }
  } else if (s21_strchr("eEfgG", reader->specification)) {
    if (reader->length.l) {
      double* dest = va_arg(args, double*);
      *dest = (double)parse_float(str, width);
    } else if (reader->length.L) {
      long double* dest = va_arg(args, long double*);
      *dest = parse_float(str, width);
    } else {
      float* dest = va_arg(args, float*);
      *dest = (float)parse_float(str, width);
    }
  }
}

int s21_sscanf(const char* str, const char* format, ...) {
  va_list args;
  va_start(args, format);

  int return_res = 0;
  int mismatch_found = 0;
  for (int processed_chars = 0; *format != '\0' && !mismatch_found;) {
    if (*format != '%') {
      if (isspace(*format)) {
        ++format;
        if (isspace(*str)) {
          ++str;
          ++processed_chars;
        }
      } else {
        if (*format == *str) {
          ++str;
          ++format;
          ++processed_chars;
        } else {
          mismatch_found = 1;
        }
      }
    } else {
      ReaderFormat reader;
      init_reader(&reader);
      parse_into_reader(&reader, format + 1);

      if (reader.specification != 'n') {
        if (s21_strlen(str) == 0) {
          va_end(args);
          return -1;
        }
        if (reader.specification != 'c' && is_blank(str) && !return_res) {
          va_end(args);
          return -1;
        }
      }

      AssignmentInfo info;
      init_assignment_info(&info, UNKNOWN, UNKNOWN, processed_chars);

      process_format_string(str, &reader, &info, args);
      processed_chars += info.source_shift;

      str += info.source_shift;
      format += reader.parsed_length + 1;
      return_res += info.return_code;
    }
  }

  va_end(args);
  return return_res;
}
