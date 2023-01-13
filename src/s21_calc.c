#include "s21_string.h"


size_t s21_strlen(const char* str) {
    size_t count = 0;
    if (str != NULL) {
        for (const char* sym = str; *sym != '\0'; ++sym, ++count) {
        }
    }
    return count;
}
