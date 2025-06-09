CC = gcc
CFLAGS = -Wall -std=c11 -O2
INCLUDES = -I./scene -I./element -I/usr/local/include
LIBS = -L/usr/local/lib -lallegro -lallegro_main -lallegro_font -lallegro_ttf -lallegro_image -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_dialog

SRC = main.c global.c GameWindow.c \
      scene/menu.c \
      scene/sceneManager.c \
      scene/gamescene.c \
      scene/successscene.c \
      scene/failscene.c \
      element/player.c \
      element/enemy.c

OBJ = $(SRC:.c=.o)

TARGET = game

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -f $(OBJ) $(TARGET)




