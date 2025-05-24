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

deps:
	@echo "Detectando sistema operacional e instalando dependências..."
	@if [ "$(UNAME_S)" = "Linux" ]; then \
		if [ -f /etc/debian_version ]; then \
			echo "Distribuição: Debian/Ubuntu"; \
			sudo apt-get update && sudo apt-get install -y build-essential pkg-config libraylib-dev libgl1-mesa-dev; \
		elif [ -f /etc/arch-release ]; then \
			echo "Distribuição: Arch Linux"; \
			sudo pacman -Sy --noconfirm base-devel raylib; \
		elif [ -f /etc/fedora-release ]; then \
			echo "Distribuição: Fedora"; \
			sudo dnf install -y @development-tools raylib-devel; \
		else \
			echo "Distribuição Linux não suportada automaticamente. Instale as dependências manualmente."; \
		fi \
	elif [ "$(UNAME_S)" = "Darwin" ]; then \
		echo "Sistema: macOS"; \
		if ! command -v brew >/dev/null 2>&1; then \
			echo "Homebrew não encontrado. Instale o Homebrew primeiro: https://brew.sh/"; \
			exit 1; \
		fi; \
		brew install raylib; \
	elif [ "$(OS)" = "Windows_NT" ]; then \
		echo "Sistema: Windows"; \
		echo "Se estiver usando MSYS2, execute: pacman -Syu mingw-w64-x86_64-raylib"; \
		echo "Caso contrário, instale as dependências manualmente."; \
	else \
		echo "Sistema operacional não suportado automaticamente. Instale as dependências manualmente."; \
	fi

.PHONY: all clean run deps
