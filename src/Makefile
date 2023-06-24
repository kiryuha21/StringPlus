CC = gcc

FLAGS = -Wall -Wextra -Werror -std=c11 -fanalyzer -pedantic

ADDITIONAL_FLAGS = -lcheck
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	ADDITIONAL_FLAGS += -lsubunit -lm
endif
#ifeq ($(UNAME_S),Darwin)
#	something for MACOS
#endif

SRC_C_FILES = s21_add.c \
			  s21_calc.c \
			  s21_cmp.c \
			  s21_cpy.c \
			  s21_search.c \
			  s21_extras.c \
			  s21_format_commons.c \
			  s21_sprintf.c \
			  s21_sscanf.c

SRC_H_FILES = s21_string.h \
			  s21_sscanf.h \
			  s21_sprintf.h

TEST_C_FILES = unit_tests.c test_commons.c
RANDOM_TESTS_FILE = random_format_tests.c
TEST_H_FILES = test_commons.h
TEST_TARGET  = s21_test

LIB = s21_string.a

REPORT_DIR = report

all: clean gcov_report $(LIB) test

test: $(SRC_C_FILES) $(SRC_H_FILES)
	cp ../materials/linters/.clang-format .
	clang-format -i *.[ch]
	rm -f .clang-format
	$(CC) $(FLAGS) $(SRC_C_FILES) $(SRC_H_FILES) $(TEST_C_FILES) $(TEST_H_FILES) -o $(TEST_TARGET) $(ADDITIONAL_FLAGS)
	./$(TEST_TARGET)

random_tests: $(SRC_C_FILES) $(SRC_H_FILES)
	cp ../materials/linters/.clang-format .
	clang-format -i *.[ch]
	rm -f .clang-format
	$(CC) $(FLAGS) $(SRC_C_FILES) $(SRC_H_FILES) $(RANDOM_TESTS_FILE) $(TEST_H_FILES) -o $(TEST_TARGET) $(ADDITIONAL_FLAGS)
	./$(TEST_TARGET)

gcov_report:
	$(CC) --coverage $(FLAGS) $(SRC_C_FILES) $(TEST_C_FILES) -o $(TEST_TARGET) $(ADDITIONAL_FLAGS)
	./$(TEST_TARGET)
	lcov -t "$(TEST_TARGET)" -o $(TEST_TARGET).info -c -d .
	genhtml -o report $(TEST_TARGET).info
	rm *.gcda *.gcno *.info

$(LIB):
	$(CC) -c $(SRC_C_FILES) $(ADDITIONAL_FLAGS)
	ar rcv $(LIB) *.o
	ranlib $(LIB)
	rm *.o

clean_test:
	rm -f $(TEST_TARGET)

clean_report:
	rm -rf $(REPORT_DIR)

clean_lib:
	rm -f $(LIB)

clean_obj:
	rm -f *.o

clean: clean_test clean_report clean_lib clean_obj
