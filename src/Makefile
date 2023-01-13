CC = gcc
FLAGS = -Wall -Wextra -Werror --std=c11 -fanalyzer -pedantic
C_FILES = s21_add.c s21_calc.c s21_cmp.c s21_cpy.c s21_search.c
ADDITIONAL = -lcheck -lsubunit -lm
H_FILES = s21_string.h
C_TEST = unit_tests.c
TARGET_TEST = s21_test

all: clean test

test: $(C_FILES) $(H_FILES)
	$(CC) $(FLAGS) $(C_FILES) $(C_TEST) $(H_FILES) -o $(TARGET_TEST) $(ADDITIONAL)

gcov_report:
	$(CC) --coverage $(FLAGS) $(C_FILES) $(C_TEST) $(H_FILES) -o $(TARGET_TEST) $(ADDITIONAL)
	./$(TARGET_TEST)
	lcov -t "$(TARGET_TEST)" -o $(TARGET_TEST).info -c -d .
	genhtml -o report $(TARGET_TEST).info

clean:
	rm -f s21_string