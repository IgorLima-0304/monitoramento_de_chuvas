CC = gcc
CFLAGS = `pkg-config --cflags --libs gtk+-3.0` -Iinclude -g
SRC = src/main.c src/previsao.c src/doacoes.c src/mensagens.c
OBJ = $(SRC:.c=.o)
TARGET = build/monitoramento

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)
