CC=gcc
CFLAGS=-Wall -Werror -Wextra
DFLAGS= -g3
PREF_SRC_GREP=./
PREF_OBJ= ../../obj/
SOURCES=$(wildcard $(PREF_SRC_GREP)*.c)
OBJ=$(SOURCES: $(PREF_SRC_GREP)/%.c=$(PREF_OBJ)/%.o)
TARGET=s21_grep
TEST_FILE=$(wildcard test_*_grep.txt)
TEST=test_func_grep.sh
DBEXE=s21
STYLE=clang-format --style=google
FORMAT= *.c

.PHONY: all test clean debug rebuild
 
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

debug: $(DBEXE)

$(DBEXE): $(SOURCES)
	$(CC) $(DFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS)  $< -o $@

test:
	$(foreach f, $(TEST_FILE), bash $(TEST) $f;)

check_style:
	$(STYLE) -n $(FORMAT)

style:
	$(STYLE) -i $(FORMAT)

cppcheck:
	cppcheck $(FORMAT)

leaks:
	leaks -atExit -- ./$(TARGET)
  
.PHONY: style leaks check_style

rebuild: clean all 

clean: 
	rm -rf $(PREF_OBJ)*.o $(TARGET) $(DBEXE) out* *.dSYM *.log