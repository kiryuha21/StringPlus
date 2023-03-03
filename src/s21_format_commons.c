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

void init_lengths(Lengths* lens) {
  lens->L = 0;
  lens->l = 0;
  lens->h = 0;
}

long long apply_signed_length(Lengths* lens, long long num) {
  long long res;
  if (lens->l == 1 || lens->L == 1) {
    res = (long)num;
  } else if (lens->l >= 2 || lens->L >= 2) {
    res = (long long)num;
  } else if (lens->h == 1) {
    res = (short)num;
  } else if (lens->h >= 2) {
    res = (signed char)num;
  } else {
    res = (int)num;
  }
  return res;
}

ull apply_unsigned_length(Lengths* lens, ull num) {
  ull res;
  if (lens->l == 1 || lens->L == 1) {
    res = (unsigned long)num;
  } else if (lens->l >= 2 || lens->L >= 2) {
    res = (unsigned long long)num;
  } else if (lens->h == 1) {
    res = (unsigned short)num;
  } else if (lens->h >= 2) {
    res = (unsigned char)num;
  } else {
    res = (unsigned int)num;
  }
  return res;
}
