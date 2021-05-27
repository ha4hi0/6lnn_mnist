CC          = gcc
CFLAGS      = -Wall -g3 -lm -I./include
LIB_DIR     = ./lib
OBJ_DIR     = ./obj
INC_DIT     = ./include
TEST_TARGET = test_main
TARGET      = main
ALL_LIBS    = $(wildcard $(LIB_DIR)/*.c)
ALL_OBJS    = $(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(ALL_LIBS))
ALL_H       = $(wildcard $(INC_DIT)/*.h)
PROGRAM     = dig-recog

.PHONY: all
all: $(TARGET) $(TEST_TARGET)

.PHONY: clean
clean:
	-rm $(TARGET) $(TEST_TARGET) $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d

run: $(TARGET)
	@bash $(PROGRAM)

test: $(TEST_TARGET)
	@bash $(PROGRAM) --test

install:
	@bash install.sh

$(TARGET): $(ALL_LIBS) $(TARGET).c $(ALL_H)
	cd obj && $(MAKE)
	$(CC) -o $(TARGET) $(ALL_OBJS) $(OBJ_DIR)/$(TARGET).o $(CFLAGS)

$(TEST_TARGET): $(ALL_LIBS) $(TEST_TARGET).c $(ALL_H)
	cd obj && $(MAKE)
	$(CC) -o $(TEST_TARGET) $(ALL_OBJS) $(OBJ_DIR)/$(TEST_TARGET).o $(CFLAGS)
