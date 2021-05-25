CC	= gcc
CFLAGS = -I. -Wall -g3 -lm
ALLSRCS = $(wildcard *.c)
ALL_H = $(wildcard *.h)
SRCS = $(filter-out test.c, $(ALLSRCS))
TEST_SRCS = $(filter-out main.c, $(ALLSRCS))
TARGET = main
TEST_TARGET = test_main
PROGRAM = dig-recog
MATRIX_DATAS = A.data
MNIST_DATAS = $(wildcard qmnist*)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	-rm $(TARGET) $(TEST_TARGET)

.PHONY: clean-all
clean-all:
	-rm $(TARGET) $(TEST_TARGET) $(MATRIX_DATAS)

run: $(TARGET) $(MNIST_DATAS)
	@`bash $(PROGRAM)`

test: $(TEST_TARGET)
	@`bash $(PROGRAM) --test`

install:
	@`bash install.sh`

$(TARGET): $(SRCS) $(ALL_H)
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS)

$(TEST_TARGET): $(TEST_SRCS) $(ALL_H)
	$(CC) -o $(TEST_TARGET) $(TEST_SRCS) $(CFLAGS)

