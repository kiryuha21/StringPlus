#include "s21_string.h"

int s21_strcmp(const char* first, const char* second) {
    while (*first != '\0' && *first == *second) {
        ++first;
        ++second;
    }

    return *(const unsigned char*)first - *(const unsigned char*)second;
}