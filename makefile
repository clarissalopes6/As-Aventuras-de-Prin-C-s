
TARGET = jogo


SRC = jogo.c


CFLAGS = -Wall

LIBS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

$(TARGET): $(SRC)
	gcc $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
