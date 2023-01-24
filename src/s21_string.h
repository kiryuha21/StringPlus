#ifndef C2_S21_STRINGPLUS_0_S21_STRING_H
#define C2_S21_STRINGPLUS_0_S21_STRING_H

#ifndef NULL
#define NULL ((void*)0)
#endif

#define ASTERISK (-20)
#define UNKNOWN (-10)

typedef unsigned long size_t;

typedef struct IntStringMap {
  int key;
  char* value;
} IntStringMap;

// TODO: wrap into smth like #ifdef linux...
static const IntStringMap sys_errlist[] = {
    {0, "Success"},
    {1, "Operation not permitted"},
    {2, "No such file or directory"},
    {3, "No such process"},
    {4, "Interrupted system call"},
    {5, "Input/output error"},
    {6, "No such device or address"},
    {7, "Argument list too long"},
    {8, "Exec format error"},
    {9, "Bad file descriptor"},
    {10, "No child processes"},
    {11, "Resource temporarily unavailable"},
    {12, "Cannot allocate memory"},
    {13, "Permission denied"},
    {14, "Bad address"},
    {15, "Block device required"},
    {16, "Device or resource busy"},
    {17, "File exists"},
    {18, "Invalid cross-device link"},
    {19, "No such device"},
    {20, "Not a directory"},
    {21, "Is a directory"},
    {22, "Invalid argument"},
    {23, "Too many open files in system"},
    {24, "Too many open files"},
    {25, "Inappropriate ioctl for device"},
    {26, "Text file busy"},
    {27, "File too large"},
    {28, "No space left on device"},
    {29, "Illegal seek"},
    {30, "Read-only file system"},
    {31, "Too many links"},
    {32, "Broken pipe"},
    {33, "Numerical argument out of domain"},
    {34, "Numerical result out of range"},
    {35, "Resource deadlock avoided"},
    {36, "File name too long"},
    {37, "No locks available"},
    {38, "Function not implemented"},
    {39, "Directory not empty"},
    {40, "Too many levels of symbolic links"},
    {11, "Resource temporarily unavailable"},
    {42, "No message of desired type"},
    {43, "Identifier removed"},
    {44, "Channel number out of range"},
    {45, "Level 2 not synchronized"},
    {46, "Level 3 halted"},
    {47, "Level 3 reset"},
    {48, "Link number out of range"},
    {49, "Protocol driver not attached"},
    {50, "No CSI structure available"},
    {51, "Level 2 halted"},
    {52, "Invalid exchange"},
    {53, "Invalid request descriptor"},
    {54, "Exchange full"},
    {55, "No anode"},
    {56, "Invalid request code"},
    {57, "Invalid slot"},
    {35, "Resource deadlock avoided"},
    {59, "Bad font file format"},
    {60, "Device not a stream"},
    {61, "No data available"},
    {62, "Timer expired"},
    {63, "Out of streams resources"},
    {64, "Machine is not on the network"},
    {65, "Package not installed"},
    {66, "Object is remote"},
    {67, "Link has been severed"},
    {68, "Advertise error"},
    {69, "Srmount error"},
    {70, "Communication error on send"},
    {71, "Protocol error"},
    {72, "Multihop attempted"},
    {73, "RFS specific error"},
    {74, "Bad message"},
    {75, "Value too large for defined data type"},
    {76, "Name not unique on network"},
    {77, "File descriptor in bad state"},
    {78, "Remote address changed"},
    {79, "Can not access a needed shared library"},
    {80, "Accessing a corrupted shared library"},
    {81, ".lib section in a.out corrupted"},
    {82, "Attempting to link in too many shared libraries"},
    {83, "Cannot exec a shared library directly"},
    {84, "Invalid or incomplete multibyte or wide character"},
    {85, "Interrupted system call should be restarted"},
    {86, "Streams pipe error"},
    {87, "Too many users"},
    {88, "Socket operation on non-socket"},
    {89, "Destination address required"},
    {90, "Message too long"},
    {91, "Protocol wrong type for socket"},
    {92, "Protocol not available"},
    {93, "Protocol not supported"},
    {94, "Socket type not supported"},
    {95, "Operation not supported"},
    {96, "Protocol family not supported"},
    {97, "Address family not supported by protocol"},
    {98, "Address already in use"},
    {99, "Cannot assign requested address"},
    {100, "Network is down"},
    {101, "Network is unreachable"},
    {102, "Network dropped connection on reset"},
    {103, "Software caused connection abort"},
    {104, "Connection reset by peer"},
    {105, "No buffer space available"},
    {106, "Transport endpoint is already connected"},
    {107, "Transport endpoint is not connected"},
    {108, "Cannot send after transport endpoint shutdown"},
    {109, "Too many references: cannot splice"},
    {110, "Connection timed out"},
    {111, "Connection refused"},
    {112, "Host is down"},
    {113, "No route to host"},
    {114, "Operation already in progress"},
    {115, "Operation now in progress"},
    {116, "Stale file handle"},
    {117, "Structure needs cleaning"},
    {118, "Not a XENIX named type file"},
    {119, "No XENIX semaphores available"},
    {120, "Is a named type file"},
    {121, "Remote I/O error"},
    {122, "Disk quota exceeded"},
    {123, "No medium found"},
    {124, "Wrong medium type"},
    {125, "Operation canceled"},
    {126, "Required key not available"},
    {127, "Key has expired"},
    {128, "Key has been revoked"},
    {129, "Key was rejected by service"},
    {130, "Owner died"},
    {131, "State not recoverable"},
    {132, "Operation not possible due to RF-kill"},
    {133, "Memory page has hardware error"},
    {95, "Operation not supported"}};
static const int sys_nerr = sizeof(sys_errlist) / sizeof(sys_errlist[0]);

// s21_search.c:
void* s21_memchr(const void* str, int c, size_t n);
char* s21_strchr(const char* src, int sym);
char* s21_strpbrk(const char* str1, const char* str2);
char* s21_strrchr(const char* str, int c);
char* s21_strstr(const char* haystack, const char* needle);

// s21_add.c:
char* s21_strtok(char* restrict str, const char* restrict delim);
char* s21_strerror(int errnum);
char* s21_strcat(char* restrict dest, const char* restrict src);
char* s21_strncat(char* dest, const char* src, size_t n);

// s21_cpy.c:
void* s21_memcpy(void* dest, const void* src, size_t n);
void* s21_memmove(void* dest, const void* src, size_t n);
void* s21_memset(void* str, int c, size_t n);
char* s21_strcpy(char* restrict dest, const char* restrict src);
char* s21_strncpy(char* dest, const char* src, size_t n);

// s21_cmp.c:
int s21_memcmp(const void* str1, const void* str2, size_t n);
int s21_strcmp(const char* str1, const char* str2);
int s21_strncmp(const char* str1, const char* str2, size_t n);

// s21_calc.c:
size_t s21_strlen(const char* str);
size_t s21_strspn(const char* str1, const char* str2);
size_t s21_strcspn(const char* str1, const char* str2);

// s21_extras.c
void* s21_to_upper(const char* str);
void* s21_to_lower(const char* str);
void* s21_insert(const char* src, const char* str, size_t start_index);
void* s21_trim(const char* src, const char* trim_chars);

static const char specifications[] = {'c', 'd', 'i', 'e', 'E', 'f', 'g', 'G',
                                      'o', 's', 'u', 'x', 'X', 'p', 'n', '%'};

static const char lengths[] = {'h', 'l', 'L'};

static const char writer_flags[] = {'-', '+', ' ', '#', '0'};

typedef struct StringVector {
  int size;
  char** strings;
} StringVector;

void init_vector(StringVector* vec);
void push_back(StringVector* vec, char* string);
void clear(StringVector* vec);

typedef struct WriterFormat {
  StringVector flags;
  char specification;
  char length;
  int width;      // could be ASTERISK
  int precision;  // could be ASTERISK
} WriterFormat;

void init_writer(WriterFormat* writer);
void parse_into_writer(WriterFormat* writer,
                       const char* src);  // src = pointer to '%'

typedef struct ReaderFormat {
  int skip_assignment;  // true if asterisk before specification
  int width;
  char length;
  char specification;
} ReaderFormat;

void init_reader(ReaderFormat* reader);
void parse_into_reader(ReaderFormat* reader,
                       const char* src);  // src = pointer to '%'

// s21_format_functions.c:
int s21_sscanf(const char* str, const char* format, ...);
int s21_sprintf(char* str, const char* format, ...);

#endif  // C2_S21_STRINGPLUS_0_S21_STRING_H
