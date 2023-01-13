#include "s21_string.h"

size_t s21_strlen(const char* str) {
    size_t count = 0;
    if (str != NULL) {
        for (const char *sym = str; *sym != '\0'; ++sym, ++count) {}
    }
    return count;
}

int s21_strcmp(const char* first, const char* second) {
    while (*first != '\0' && *first == *second) {
        ++first;
        ++second;
    }

    // вернуть разницу ASCII после преобразования `char*` в `unsigned char*`
    return *(const unsigned char*)first - *(const unsigned char*)second;
}

char* s21_strcpy(char* restrict destination, const char* restrict source) {
    if (destination == NULL) {
        return NULL;
    }

    char* ptr = destination;  // pointer to the dst start

    while (*source != '\0') {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';

    return ptr;
}

char *s21_strcat(char *restrict destination, const char *restrict source) {
    size_t dst_len = s21_strlen(destination);
    return s21_strcpy(destination + dst_len, source) - dst_len;
}

char* s21_strchr(const char* src, int sym) {
    char* res = NULL;
    int stop = 0;
    while (*src != '\0' && !stop) {
        if (*src == sym) {
            res = (char*)src;
            stop = 1;
        } else {
            ++src;
        }
    }
    return res;
}

int compare(const char* haystack, const char* needle) {
    int return_code = 1;
    int stop = 0;

    while (*haystack && *needle && !stop) {
        if (*haystack != *needle) {
            return_code = 0;
            stop = 1;
        }
        ++haystack;
        ++needle;
    }

    if (*needle != '\0') {
        return_code = 0;
    }
    return return_code;
}

char* s21_strstr(const char* haystack, const char* needle) {
    int stop = 0;
    char* result = NULL;

    while (*haystack != '\0' && !stop) {
        if ((*haystack == *needle) && compare(haystack, needle)) {
            result = (char*)haystack;
            stop = 1;
        }
        ++haystack;
    }

    return result;
}

char* s21_strtok(char* restrict str, const char *delim) {
    static char* backup;
    char* return_value = NULL;

    if (str == NULL) {
        str = backup;
    }
    if (str != NULL) {
        int stop = 0;
        while (s21_strchr(delim, *str) && !stop) {
            ++str;
            if (*str == '\0') {
                stop = 1;
            }
        }
        if (!stop) {
            return_value = str;
            while (!stop) {
                if (*str == '\0') {
                    backup = str;
                    stop = 1;
                } else if (s21_strchr(delim, *str)) {
                    *str = '\0';
                    backup = str + 1;
                    stop = 1;
                } else {
                    ++str;
                }
            }
        }
    }

    return return_value;
}
