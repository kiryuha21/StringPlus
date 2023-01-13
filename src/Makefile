CC = gcc

FLAGS = -Wall -Werror -Wextra

SRC = s21_string.c

TEST = unit_tests.c s21_string.c

STANDARD = -std=c11

TARGET = s21_string
TEST_TARGET = s21_test

# required: all, clean, test, s21_string.a, gcov_report

all: clean s21_string test #s21_string.a gcov_report

test:
	$(CC) $(STANDARD) $(TEST) -lcheck -lsubunit -lm -o $(TEST_TARGET)

#s21_string.a:

#gcov_report:

clean:
	rm -f $(TARGET)

s21_string:
	$(CC) $(FLAGS) $(STANDARD) $(SRC) -o $(TARGET)

build_no_flags:
	$(CC) $(STANDARD) $(SRC) -o $(TARGET)

run:
	./$(TARGET)
