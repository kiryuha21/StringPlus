CC = gcc

FLAGS = -Wall -Wextra -Werror --std=c11 -fanalyzer -pedantic
ADDITIONAL_FLAGS = -lcheck -lsubunit -lm

SRC_FILES = s21_add.c s21_calc.c s21_cmp.c s21_cpy.c s21_search.c
HEADER_FILES = s21_string.h
TEST_FILES = unit_tests.c

TEST_TARGET = s21_test

all: clean test

test: $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(FLAGS) $(SRC_FILES) $(TEST_FILES) $(HEADER_FILES) -o $(TEST_TARGET) $(ADDITIONAL_FLAGS)

gcov_report:
	$(CC) --coverage $(FLAGS) $(SRC_FILES) $(TEST_FILES) $(HEADER_FILES) -o $(TEST_TARGET) $(ADDITIONAL_FLAGS)
	./$(TEST_TARGET)
	lcov -t "$(TEST_TARGET)" -o $(TEST_TARGET).info -c -d .
	genhtml -o report $(TEST_TARGET).info

clean:
	rm -f $(TEST_TARGET)