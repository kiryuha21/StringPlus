CC = gcc

FLAGS = -Wall -Wextra -Werror --std=c11 -fanalyzer -pedantic
ADDITIONAL_FLAGS = -lcheck -lsubunit -lm

SRC_C_FILES = s21_add.c s21_calc.c s21_cmp.c s21_cpy.c s21_search.c
SRC_H_FILES = s21_string.h
TEST_C_FILES = unit_tests.c test_commons.c
TEST_H_FILES = test_commons.h
REPORT_DIR = report

TEST_TARGET = s21_test

all: clean test

test: $(SRC_C_FILES) $(SRC_H_FILES)
	$(CC) $(FLAGS) $(SRC_C_FILES) $(SRC_H_FILES) $(TEST_C_FILES) $(TEST_H_FILES) -o $(TEST_TARGET) $(ADDITIONAL_FLAGS)

gcov_report:
	$(CC) --coverage $(FLAGS) $(SRC_C_FILES) $(TEST_C_FILES) $(SRC_H_FILES) -o $(TEST_TARGET) $(ADDITIONAL_FLAGS)
	./$(TEST_TARGET)
	lcov -t "$(TEST_TARGET)" -o $(TEST_TARGET).info -c -d .
	genhtml -o report $(TEST_TARGET).info
	rm *.gcda *.gcno *.info

clean_test:
	rm -f $(TEST_TARGET)

clean_report:
	rm -rf $(REPORT_DIR)

#clean_lib:

clean: clean_test clean_report #clean_lib