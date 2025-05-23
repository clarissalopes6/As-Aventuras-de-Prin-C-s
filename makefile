TARGET = jogo
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Icode
SDIR = code

SOURCES_NAMES = main princesa ataque vilao orbe highscore jogo
OBJ_FILES = $(addsuffix .o, $(SOURCES_NAMES))

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif
ifeq ($(UNAME_S),Darwin)
    LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
endif

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean run