CC	= gcc
CFLAGS = -I. -Wall -g3
SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
DEPS = $(OBJS:%.o=%.d)
ALL_H = $(wildcard *.h)
TARGET = main
PROGRAM = dig-recog
MATRIX_DATAS = A.data
MNIST_DATAS = $(wildcard qmnist*)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	-rm $(TARGET) $(OBJS) $(DEPS)

.PHONY: clean-all
clean-all:
	-rm $(TARGET) $(OBJS) $(MATRIX_DATAS) $(DEPS)

-include $(DEPS)

run: $(TARGET) $(MNIST_DATAS)
	@`bash $(PROGRAM)`

test: $(TARGET)
	@`bash $(PROGRAM) --test`

install:
	@`bash install.sh`

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

%.d: %.c $(ALL_H)
	$(info GEN $@)
	@$(CC) -MM $(CFLAGS) $< -MF $@

%.o: %.c
	$(CC) -c $< $(CFLAGS)
