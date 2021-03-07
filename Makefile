SRC_FILES = $(wildcard src/*.c wildcard src/*.cpp)
BIN_DIR = bin
EXEC_FILE = game
LIBS = dl GL glfw m freetype

INCLUDES = -I/usr/include/freetype2 -I/usr/include/libpng16

libstring = $(foreach lib, $(LIBS), -l$(lib))

$(BIN_DIR)/$(EXEC_FILE): $(SRC_FILES)
	gcc $(SRC_FILES) -o $@ $(libstring) -ggdb -w -fcompare-debug-second $(INCLUDES)


run: $(BIN_DIR)/$(EXEC_FILE)
	./$^

clean:
	rm $(BIN_DIR)/*