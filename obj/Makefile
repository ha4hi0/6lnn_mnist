CC         =gcc
CFLAGS     = -Wall -g3 -lm -I../include
LIB_DIR    =./../lib
INCLUDE_DIR=./../include
ALL_LIBS   = $(wildcard $(LIB_DIR)/*.c)
ALL_OBJS   = $(patsubst $(LIB_DIR)/%.c, %.o, $(ALL_LIBS))
ALL_DEPS   = $(patsubst $(LIB_DIR)/%.c, %.d, $(ALL_LIBS))
ALL_H      = $(wildcard $(INCLUDE_DIR)/*.h)

.PHONY: all
all: $(ALL_OBJS) test_main.o main.o

test_main.o: ../test_main.c
	-$(CC) -c $< -o $@ $(CFLAGS)

test_main.d: ../test_main.c $(ALL_H)
	-$(CC) -MM $< -MF $@ $(CFLAGS)

main.o: ../main.c
	-$(CC) -c $< -o $@ $(CFLAGS)

main.d: ../main.c
	-$(CC) -c $< -o $@ $(CFLAGS)

%.o: $(LIB_DIR)/%.c
	-$(CC) -c $< -o $@ $(CFLAGS)

%.d: $(LIB_DIR)/%.c $(ALL_H)
	-$(CC) -MM $< -MF $@ $(CFLAGS)

-include $(ALL_DEPS)
