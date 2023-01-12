CC = gcc

FLAGS = -Wall -Werror -Wextra

SRC = s21_cat.c

STANDARD = -std=c11

TARGET = s21_string

# required: all, clean, test, s21_string.a, gcov_report

all: clean s21_string #test s21_string.a gcov_report

#test:

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
