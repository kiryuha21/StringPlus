#include <ctype.h>

#include "s21_string.h"

const char* specifications = "cdieEfgGosuxXpn%";
const char* writer_flags = "-+ #0";
const char* lengths = "hlL";
const char* decimal_places = "0123456789ABCDEF";

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
